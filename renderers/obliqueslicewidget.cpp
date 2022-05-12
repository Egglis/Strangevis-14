#include "obliqueslicewidget.h"

#include "../geometry.h"

#include <QPainter>
#include <QVector3D>

ObliqueSliceRenderWidget::ObliqueSliceRenderWidget(
    std::unique_ptr<ITextureStore>& textureStore,
    const std::shared_ptr<ISharedProperties> properties, QWidget* parent,
    Qt::WindowFlags f)
    : QOpenGLWidget(parent, f),
      m_textureStore(textureStore), m_properties{properties},
      m_cubePlaneIntersection{m_properties->clippingPlane().plane()},
      m_prevRotation{0}, m_verticalFlipped{false}, m_horizontalFlipped{false},
      m_selectedPoint{0, 0}
{
    m_viewMatrix.scale(1 / sqrt(3.0));
    connect(&m_properties.get()->transferFunction(),
            &tfn::TransferProperties::colorMapChanged, [this]() { update(); });
}

void ObliqueSliceRenderWidget::initializeGL()
{
    initializeOpenGLFunctions();
    // initialize geometry
    Geometry::instance();

    if (!m_sliceProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                                ":shaders/slice-vs.glsl"))
        qDebug() << "Could not load vertex shader!";

    if (!m_sliceProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                                ":shaders/slice-fs.glsl"))
        qDebug() << "Could not load fragment shader!";

    if (!m_sliceProgram.link())
        qDebug() << "Could not link shader program!";
    auto updateObliqueSlice = [this]() {
        Geometry::instance().allocateObliqueSlice(m_cubePlaneIntersection);
    };
    connect(&m_textureStore->volume(), &Volume::volumeLoaded, this,
            [this]() { update(); });
    connect(&m_properties.get()->clippingPlane(),
            &ClippingPlaneProperties::clippingPlaneChanged,
            [this, updateObliqueSlice](const Plane& plane) {
                m_cubePlaneIntersection.changePlane(plane);
                updateObliqueSlice();
                update();
            });
    connect(&m_properties.get()->transferFunction(),
            &tfn::TransferProperties::transferFunctionChanged,
            [this]() { update(); });
    updateObliqueSlice();
    moveSelection(QPointF{width() / 2.f, height() / 2.f});
}

void ObliqueSliceRenderWidget::paintGL()
{
    m_sliceProgram.bind();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_textureStore->volume().loadingInProgress())
        return;

    paintSlice();
    m_sliceProgram.release();
    paintSelection();
}

bool ObliqueSliceRenderWidget::moveSelection(QPointF newSelection)
{
    if (newSelection == m_selectedPoint)
        return false;
    m_selectedPoint = newSelection;
    m_selectedBox = QRectF{m_selectedPoint - QPointF{5, 5},
                           m_selectedPoint + QPointF{5, 5}};
    QVector3D normalizedPoint{
        (static_cast<float>(m_selectedPoint.x() / width()) - 0.5f) * 2.f,
        -(static_cast<float>(m_selectedPoint.y() / height()) - 0.5f) * 2.f, 0.f};

    QMatrix4x4 modelViewMatrix =
        m_aspectRatioMatrix * m_viewMatrix *
        m_cubePlaneIntersection.getModelRotationMatrix() *
        m_textureStore->volume().modelMatrix();
    // QMatrix4x4 viewMatrix = m_aspectRatioMatrix * m_viewMatrix;
    m_selectedVolumePoint = modelViewMatrix.inverted().map(normalizedPoint);
    m_properties->clippingPlane().updateSelectedPoint(m_selectedVolumePoint);
    qDebug() << m_selectedVolumePoint;
    return true;
}

void ObliqueSliceRenderWidget::paintSlice()
{
    QMatrix4x4 modelViewMatrix =
        m_aspectRatioMatrix * m_viewMatrix *
        m_cubePlaneIntersection.getModelRotationMatrix() *
        m_textureStore->volume().modelMatrix();
    m_sliceProgram.setUniformValue("modelViewMatrix", modelViewMatrix);

    glActiveTexture(GL_TEXTURE0);
    m_sliceProgram.setUniformValue("volumeTexture", 0);
    m_textureStore->volume().bind();

    // Transfer Texture
    glActiveTexture(GL_TEXTURE1);
    m_sliceProgram.setUniformValue("transferFunction", 1);
    m_textureStore->transferFunction().bind();

    Geometry::instance().bindObliqueSliceIntersectionCoords();
    {
        int location = m_sliceProgram.attributeLocation("intersectionCoords");
        m_sliceProgram.enableAttributeArray(location);
        m_sliceProgram.setAttributeBuffer(location, GL_FLOAT, 0, 3,
                                          sizeof(QVector3D));
    }

    Geometry::instance().drawObliqueSlice();

    Geometry::instance().releaseObliqueSliceIntersectionCoords();

    glActiveTexture(GL_TEXTURE0);
    m_textureStore->volume().release();
    glActiveTexture(GL_TEXTURE1);
    m_textureStore->transferFunction().release();
}

void ObliqueSliceRenderWidget::paintSelection()
{
    QPainter painter{this};
    painter.setPen(Qt::red);
    painter.drawEllipse(m_selectedBox);
}

void ObliqueSliceRenderWidget::resizeGL(int w, int h)
{
    correctQuadForAspectRatio(w, h);
}

void ObliqueSliceRenderWidget::correctQuadForAspectRatio(int w, int h)
{
    m_aspectRatioMatrix.setToIdentity();
    float aspectRatio = w / static_cast<float>(h);
    m_aspectRatioMatrix.scale(1 / aspectRatio, 1, 1);
}
void ObliqueSliceRenderWidget::rotate(float degrees)
{
    QVector3D zAxis(0, 0, 1);
    m_viewMatrix.rotate(m_prevRotation, zAxis);
    m_viewMatrix.rotate(-degrees, zAxis);
    m_prevRotation = degrees;
    update();
}
void ObliqueSliceRenderWidget::flipHorizontal(bool flip)
{
    int scale = flip != m_horizontalFlipped ? -1 : 1;
    float prevRotation = m_prevRotation;
    rotate(0);
    m_viewMatrix.scale(scale, 1);
    m_horizontalFlipped = flip;
    rotate(prevRotation);
}
void ObliqueSliceRenderWidget::flipVertical(bool flip)
{
    int scale = flip != m_verticalFlipped ? -1 : 1;
    float prevRotation = m_prevRotation;
    rotate(0);
    m_viewMatrix.scale(1, scale);
    m_verticalFlipped = flip;
    rotate(prevRotation);
}

void ObliqueSliceRenderWidget::zoomCamera(float zoomFactor)
{
    m_viewMatrix.scale(zoomFactor);
    update();
}
