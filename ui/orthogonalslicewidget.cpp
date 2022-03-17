#include "orthogonalslicewidget.h"

#include "../geometry.h"

#include <QMouseEvent>
#include <QRadioButton>
#include <QCheckBox>
#include <QVector3D>
#include <QtMath>

OrthogonalSliceWidget::OrthogonalSliceWidget(
    Environment* env, std::shared_ptr<SharedProperties> properties,
    QWidget* parent, Qt::WindowFlags f)
    : QOpenGLWidget(parent, f), m_environment(env), x_properties{properties}
{
    connect(&x_properties.get()->slicingPlane(),
            &SlicingPlaneProperties::currentPlaneChanged,
            [this](SlicingPlane plane) {
                m_slicingPlane = static_cast<int>(plane);
                update();
            });

    connect(
        &x_properties.get()->slicingPlane(),
        &SlicingPlaneProperties::currentSliceChanged, [this](int slice) {
            int modelDim =
                m_environment->volume()->getDimensions()[2 - m_slicingPlane];
            if (slice > modelDim)
            {
                x_properties.get()->slicingPlane().changeSlice(modelDim);
                return;
            }
            if (slice < 0)
            {
                x_properties.get()->slicingPlane().changeSlice(0);
                return;
            }

            m_slice = modelDim ? static_cast<float>(slice) / modelDim
                               : 0; // Don't divide by 0
            qDebug() << "Slice changed to" << slice << "and fractionally to"
                     << m_slice;
            update();
        });

    connect(&x_properties.get()->slicingPlane(), &SlicingPlaneProperties::flipHorizontal, [this](bool flip){
        m_flipHorizontal = flip;
        updateModelViewMatrix();
        update();
    });
    connect(&x_properties.get()->slicingPlane(), &SlicingPlaneProperties::flipVertical, [this](bool flip){
        m_flipVertical = flip;
        updateModelViewMatrix();
        update();
    });
    updateModelViewMatrix();
}

void OrthogonalSliceWidget::initializeGL()
{
    initializeOpenGLFunctions();
    // initialize geometry
    Geometry::instance();

    if (!m_sliceProgram.addShaderFromSourceFile(
            QOpenGLShader::Vertex, ":/shaders/shaders/slice-vs.glsl"))
        qDebug() << "Could not load vertex shader!";

    if (!m_sliceProgram.addShaderFromSourceFile(
            QOpenGLShader::Fragment, ":/shaders/shaders/slice-fs.glsl"))
        qDebug() << "Could not load fragment shader!";

    if (!m_sliceProgram.link())
        qDebug() << "Could not link shader program!";
}

void OrthogonalSliceWidget::wheelEvent(QWheelEvent* p_event)
{
    QPoint deg = p_event->angleDelta();
    if (deg.y() < 0)
    {
        x_properties->slicingPlane().incrementSlice();
    }
    if (deg.y() > 0)
    {
        x_properties->slicingPlane().decrementSlice();
    }
}

void OrthogonalSliceWidget::updateModelViewMatrix()
{
    switch (m_slicingPlane)
    {
    default:
    case 0:
        m_modelViewMatrix.setToIdentity();
        break;
    case 1:
        m_modelViewMatrix =
            QMatrix4x4(-1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
            break;
    case 2:
        m_modelViewMatrix =
            QMatrix4x4(0, -1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1);
            break;
    }
    QMatrix4x4 flipMatrix{};
    flipMatrix.setToIdentity();
    if (m_flipHorizontal)
    {
        flipMatrix.setRow(0,-flipMatrix.row(0));
    }
    if (m_flipVertical)
    {
        flipMatrix.setRow(1,-flipMatrix.row(1));
    }
    m_modelViewMatrix = flipMatrix*m_modelViewMatrix;
}

void OrthogonalSliceWidget::paintGL()
{

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_sliceProgram.bind();
    m_sliceProgram.setUniformValue("slice", m_slice);
    m_sliceProgram.setUniformValue("slicingPlane", m_slicingPlane);
    m_sliceProgram.setUniformValue("modelViewMatrix", m_modelViewMatrix);

    glActiveTexture(GL_TEXTURE0);
    m_sliceProgram.setUniformValue("volumeTexture", 0);
    m_environment->volume()->bind();

    Geometry::instance().bindQuad();

    int location = m_sliceProgram.attributeLocation("vertexPosition");
    m_sliceProgram.enableAttributeArray(location);
    m_sliceProgram.setAttributeBuffer(location, GL_FLOAT, 0, 3,
                                      sizeof(QVector3D));

    Geometry::instance().drawQuad();

    glActiveTexture(GL_TEXTURE0);
    m_environment->volume()->release();

    m_sliceProgram.release();
}

OrthogonalSliceToolbar::OrthogonalSliceToolbar(
    std::shared_ptr<SharedProperties> properties, QWidget* parent)
    : QWidget(parent), x_properties{properties}, m_layout(this)
{
    QRadioButton* axialButton = new QRadioButton("Axial", this);     // xy plane
    QRadioButton* coronalButton = new QRadioButton("Coronal", this); // xz plane
    QRadioButton* sagittalButton =
        new QRadioButton("Sagittal", this); // yz plane

    m_planeSelectionLayout.addWidget(axialButton);
    m_planeSelectionLayout.addWidget(coronalButton);
    m_planeSelectionLayout.addWidget(sagittalButton);
    m_layout.addLayout(&m_planeSelectionLayout);

    connect(axialButton, &QRadioButton::toggled, [this](bool checked) {
        if (checked)
        {
            x_properties->slicingPlane().changePlane(SlicingPlane::Axial);
        }
    });
    connect(coronalButton, &QRadioButton::toggled, [this](bool checked) {
        if (checked)
        {
            x_properties->slicingPlane().changePlane(SlicingPlane::Coronal);
        }
    });
    connect(sagittalButton, &QRadioButton::toggled, [this](bool checked) {
        if (checked)
        {
            x_properties->slicingPlane().changePlane(SlicingPlane::Sagittal);
        }
    });
    axialButton->toggle();

    QCheckBox* flipHorizontalButton = new QCheckBox("Flip Horizontal", this);
    QCheckBox* flipVerticalButton = new QCheckBox("Flip Vertical", this);

    m_flipSelectionLayout.addWidget(flipHorizontalButton);
    m_flipSelectionLayout.addWidget(flipVerticalButton);
    m_layout.addLayout(&m_flipSelectionLayout);

    connect(flipHorizontalButton, &QCheckBox::stateChanged, [this](int state){
        x_properties->slicingPlane().flipSliceHorizontal(state == Qt::Checked);
    });
    connect(flipVerticalButton, &QCheckBox::stateChanged, [this](int state){
        x_properties->slicingPlane().flipSliceVertical(state == Qt::Checked);
    });
}
