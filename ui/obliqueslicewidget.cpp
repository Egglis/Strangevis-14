#include "obliqueslicewidget.h"

#include "../geometry.h"

#include <QVector3D>

ObliqueSliceRenderWidget::ObliqueSliceRenderWidget(
    const std::shared_ptr<ITextureStore> textureStore,
    const std::shared_ptr<const ISharedProperties> properties, QWidget* parent,
    Qt::WindowFlags f)
    : QOpenGLWidget(parent, f), m_textureStore(textureStore), m_properties{properties},
      m_cubePlaneIntersection(m_properties->clippingPlane().plane()),
      m_prevRotation{0}, m_verticalFlipped{false}, m_horizontalFlipped{false}
{
    m_modelViewMatrix.scale(1 / sqrt(3.0));
    connect(&m_properties.get()->colorMap(),
            &tfn::TransferProperties::transferFunctionChanged,
            [this]() { update(); });
}

void ObliqueSliceRenderWidget::initializeGL()
{
    initializeOpenGLFunctions();
    // initialize geometry
    Geometry::instance();

    if (!m_sliceProgram.addShaderFromSourceFile(
            QOpenGLShader::Vertex, ":shaders/slice-vs.glsl"))
        qDebug() << "Could not load vertex shader!";

    if (!m_sliceProgram.addShaderFromSourceFile(
            QOpenGLShader::Fragment, ":shaders/slice-fs.glsl"))
        qDebug() << "Could not load fragment shader!";

    if (!m_sliceProgram.link())
        qDebug() << "Could not link shader program!";
    auto updateObliqueSlice = [this]() {
        Geometry::instance().allocateObliqueSlice(m_cubePlaneIntersection);
    };
    connect(&m_properties.get()->clippingPlane(),
            &ClippingPlaneProperties::clippingPlaneChanged,
            [this, updateObliqueSlice](const Plane& plane) {
                m_cubePlaneIntersection.changePlane(plane);
                updateObliqueSlice();
                update();
            });
    updateObliqueSlice();
}

void ObliqueSliceRenderWidget::paintGL()
{

    Geometry::instance().allocateObliqueSlice(m_cubePlaneIntersection);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_sliceProgram.bind();

    m_sliceProgram.setUniformValue("modelViewMatrix", m_modelViewMatrix);

    glActiveTexture(GL_TEXTURE0);
    m_sliceProgram.setUniformValue("volumeTexture", 0);
    m_textureStore->volume().bind();

    // Transfer Texture
    glActiveTexture(GL_TEXTURE1);
    m_sliceProgram.setUniformValue("transferFunction", 1);
    m_textureStore->transferFunction().bind();
    Geometry::instance().bindObliqueSliceVertex();

    {
        int location = m_sliceProgram.attributeLocation("vertexPosition");
        m_sliceProgram.enableAttributeArray(location);
        m_sliceProgram.setAttributeBuffer(location, GL_FLOAT, 0, 2,
                                          sizeof(QVector2D));
    }

    Geometry::instance().bindObliqueSliceTexCoord();
    {
        int location = m_sliceProgram.attributeLocation("texCoord");
        m_sliceProgram.enableAttributeArray(location);
        m_sliceProgram.setAttributeBuffer(location, GL_FLOAT, 0, 3,
                                          sizeof(QVector3D));
    }

    Geometry::instance().drawObliqueSlice();

    glActiveTexture(GL_TEXTURE0);
    m_textureStore->volume().release();
    glActiveTexture(GL_TEXTURE1);
    m_textureStore->transferFunction().release();

    m_sliceProgram.release();
}

void ObliqueSliceRenderWidget::updateTransferTexture(tfn::ColorMap cmap)
{
    m_textureStore->transferFunction().setColorMap(cmap);
    update();
}

void ObliqueSliceRenderWidget::rotate(float degrees)
{
    QVector3D zAxis(0, 0, 1);
    m_modelViewMatrix.rotate(m_prevRotation, zAxis);
    m_modelViewMatrix.rotate(-degrees, zAxis);
    m_prevRotation = degrees;
    update();
}
void ObliqueSliceRenderWidget::flipHorizontal(bool flip)
{
    int scale = flip != m_horizontalFlipped ? -1 : 1;
    float prevRotation = m_prevRotation;
    rotate(0);
    m_modelViewMatrix.scale(scale, 1);
    m_horizontalFlipped = flip;
    rotate(prevRotation);
}
void ObliqueSliceRenderWidget::flipVertical(bool flip)
{
    int scale = flip != m_verticalFlipped ? -1 : 1;
    float prevRotation = m_prevRotation;
    rotate(0);
    m_modelViewMatrix.scale(1, scale);
    m_verticalFlipped = flip;
    rotate(prevRotation);
}
ObliqueSliceRotationWidget::ObliqueSliceRotationWidget(
    const std::shared_ptr<ISharedProperties>& properties, QWidget* parent)
    : m_flipHorizontalCheckbox{tr("Flip Horizontal")},
      m_flipVerticalCheckbox{tr("Flip Vertical")}, m_layout{this},
      m_parameterWidget(properties, this)
{
    connect(&m_flipHorizontalCheckbox, &QCheckBox::stateChanged, this,
            &ObliqueSliceRotationWidget::flipHorizontal);
    connect(&m_flipVerticalCheckbox, &QCheckBox::stateChanged, this,
            &ObliqueSliceRotationWidget::flipVertical);
    QHBoxLayout* checkboxLayout = new QHBoxLayout();
    checkboxLayout->addWidget(&m_flipHorizontalCheckbox);
    checkboxLayout->addWidget(&m_flipVerticalCheckbox);
    m_layout.addLayout(checkboxLayout);
    m_layout.addWidget(&m_parameterWidget);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
}

ObliqueSliceWidget::ObliqueSliceWidget(
    const std::shared_ptr<ITextureStore>& textureStore,
    const std::shared_ptr<ISharedProperties>& properties, QWidget* parent,
    Qt::WindowFlags f)
    : QWidget(parent)
{
    m_renderWidget = new ObliqueSliceRenderWidget(textureStore, properties, this, f);
    m_dial = new QDial(this);
    QHBoxLayout* hl = new QHBoxLayout();
    QVBoxLayout* vl = new QVBoxLayout();
    vl->addStretch(4);
    vl->addWidget(m_dial, 1);
    hl->addStretch(4);
    hl->addLayout(vl, 1);
    QWidget* w = new QWidget();
    w->setLayout(hl);
    m_layout = new QStackedLayout(this);
    m_layout->setStackingMode(QStackedLayout::StackAll);
    m_layout->addWidget(w);
    m_layout->addWidget(m_renderWidget);

    m_dial->setRange(0, 359);
    connect(m_dial, &QDial::valueChanged, m_renderWidget,
            &ObliqueSliceRenderWidget::rotate);
}
