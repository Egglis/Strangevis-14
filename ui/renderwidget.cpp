#include "renderwidget.h"

#include "../geometry.h"
#include "../transfertexture.h"
#include "parameterwidget.h"
#include "transferfunctionwidget.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QtMath>


RenderWidget::RenderWidget(std::shared_ptr<Environment> env,
                           std::shared_ptr<SharedProperties> properties,
                           QWidget* parent, Qt::WindowFlags f)
    : QOpenGLWidget(parent, f), m_environment{env}, m_properties{properties}
{
    m_modelViewMatrix.setToIdentity();
    m_modelViewMatrix.translate(0.0, 0.0, -2.0 * sqrt(3.0));
    connect(&m_properties.get()->clippingPlane(),
            &ClippingPlaneProperties::clippingPlaneChanged,
            [this](Plane plane) { update(); });
    connect(&m_properties.get()->gradientMethod(),
            &GradientProperties::gradientMethodChanged,
            [this](GradientMethod method) { update(); });

    connect(m_environment->volume(), &Volume::dimensionsChanged, this,
            &RenderWidget::updateBoxScalingMatrix);

    connect(&m_properties.get()->colorMap(),
            &TransferProperties::transferTextureChanged, this,
            &RenderWidget::updateTransferTexture);
}

void RenderWidget::mousePressEvent(QMouseEvent* p_event)
{
    m_currentX = static_cast<qreal>(p_event->x());
    m_currentY = static_cast<qreal>(p_event->y());

    m_previousX = m_currentX;
    m_previousY = m_currentY;
}

void RenderWidget::mouseMoveEvent(QMouseEvent* p_event)
{
    m_currentX = static_cast<qreal>(p_event->x());
    m_currentY = static_cast<qreal>(p_event->y());

    if (p_event->buttons() & Qt::LeftButton)
    {
        if (m_currentX != m_previousX || m_currentY != m_previousY)
        {
            updateModelViewMatrix();
        }
    }
    m_previousX = m_currentX;
    m_previousY = m_currentY;

    update();
}

// Scrolling wheel event
void RenderWidget::wheelEvent(QWheelEvent* p_event)
{
    QPoint deg = p_event->angleDelta();
    float zoomScale = 1.0;

    if (deg.y() < 0)
    {
        zoomScale = zoomScale / 1.1;
    }
    if (deg.y() > 0)
    {
        zoomScale = zoomScale * 1.1;
    }

    m_modelViewMatrix.scale(zoomScale);

    update();
}

void RenderWidget::updateModelViewMatrix()
{
    QVector3D va = arcballVector(m_previousX, m_previousY);
    QVector3D vb = arcballVector(m_currentX, m_currentY);

    if (va != vb)
    {
        qreal angle =
            acos(qMax(-1.0f, qMin(1.0f, QVector3D::dotProduct(va, vb))));
        QVector3D axis = QVector3D::crossProduct(va, vb);

        QMatrix4x4 inverseModelViewMatrix = m_modelViewMatrix.inverted();
        QVector4D transformedAxis =
            inverseModelViewMatrix * QVector4D(axis, 0.0f);
        m_modelViewMatrix.rotate(qRadiansToDegrees(angle),
                                 transformedAxis.toVector3D());
    }
}

void RenderWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_CLIP_DISTANCE0);
    // initialize geometry
    Geometry::instance();

    if (!m_cubeProgram.addShaderFromSourceFile(
            QOpenGLShader::Vertex, ":/shaders/shaders/cube-vs.glsl"))
        qDebug() << "Could not load vertex shader!";

    if (!m_cubeProgram.addShaderFromSourceFile(
            QOpenGLShader::Fragment, ":/shaders/shaders/cube-fs.glsl"))
        qDebug() << "Could not load fragment shader!";

    if (!m_cubeProgram.link())
        qDebug() << "Could not link shader program!";
}

void RenderWidget::resizeGL(int w, int h)
{
    qreal aspectRatio = static_cast<qreal>(w) / static_cast<qreal>(h);

    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(m_fov, aspectRatio, m_nearPlane, m_farPlane);
}

void RenderWidget::paintGL()
{
    int location = -1;

    glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    QVector4D planeEquation = {
        m_properties->clippingPlane().plane().normal().normalized(),
        m_properties->clippingPlane().plane().d()};

    QMatrix4x4 modelViewProjectionMatrix =
        m_projectionMatrix * m_modelViewMatrix * m_boxScalingMatrix;

    m_cubeProgram.bind();
    location = m_cubeProgram.uniformLocation("clippingPlaneEquation");
    m_cubeProgram.setUniformValue(location, planeEquation);
    location = m_cubeProgram.uniformLocation("modelViewProjectionMatrix");
    m_cubeProgram.setUniformValue(location, modelViewProjectionMatrix);
    location = m_cubeProgram.uniformLocation("gradientMethod");
    m_cubeProgram.setUniformValue(
        location, static_cast<int>(m_properties->gradientMethod().method()));

    auto [width, height, depth] = m_environment->volume()->getDimensions();
    location = m_cubeProgram.uniformLocation("width");
    m_cubeProgram.setUniformValue(location, static_cast<int>(width));
    location = m_cubeProgram.uniformLocation("height");
    m_cubeProgram.setUniformValue(location, static_cast<int>(height));
    location = m_cubeProgram.uniformLocation("depth");
    m_cubeProgram.setUniformValue(location, static_cast<int>(depth));

    glActiveTexture(GL_TEXTURE0);
    m_cubeProgram.setUniformValue("volumeTexture", 0);
    m_environment->volume()->bind();

    // Transfer Texture
    glActiveTexture(GL_TEXTURE1);
    m_cubeProgram.setUniformValue("transferTexture", 1);
    m_environment->transferTexture()->bind();

    Geometry::instance().bindCube();

    location = m_cubeProgram.attributeLocation("vertexPosition");
    m_cubeProgram.enableAttributeArray(location);
    m_cubeProgram.setAttributeBuffer(location, GL_FLOAT, 0, 3,
                                     sizeof(QVector3D));

    Geometry::instance().drawCube();
    glActiveTexture(GL_TEXTURE0);

    m_environment->transferTexture()->release();
    m_environment->volume()->release();
    m_cubeProgram.release();
}

QVector3D RenderWidget::arcballVector(qreal x, qreal y)
{
    QVector3D p = QVector3D(
        2.0f * static_cast<float>(x) / static_cast<float>(this->width()) - 1.0f,
        -2.0f * static_cast<float>(y) / static_cast<float>(this->height()) +
            1.0f,
        0.0);

    float length2 = p.x() * p.x() + p.y() * p.y();

    if (length2 < 1.0f)
    {
        p.setZ(sqrtf(1.0f - length2));
    }
    else
    {
        p.normalize();
    }
    return p;
}
void RenderWidget::updateBoxScalingMatrix()
{
    m_boxScalingMatrix.setToIdentity();
    auto dims = m_environment->volume()->getDimensions();
    auto maxDim = std::max(dims.x(), std::max(dims.y(), dims.z()));
    if (maxDim)
    {
        m_boxScalingMatrix.scale(dims / maxDim);
    }
}

void RenderWidget::updateTransferTexture(ColorMap cmap)
{
    m_environment->transferTexture()->setColorMap(cmap);
    update();
}

ExtendedParameterWidget::ExtendedParameterWidget(
    const std::shared_ptr<SharedProperties>& properties, QWidget* parent)
    : QWidget(parent), m_parameterWidget(properties, this),
      m_transferWidget(properties, this), m_layout{this}
{
    m_layout.addWidget(&m_transferWidget);
    m_layout.addWidget(&m_parameterWidget);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
}
