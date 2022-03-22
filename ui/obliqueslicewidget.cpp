#include "obliqueslicewidget.h"

#include "../geometry.h"

#include <QVector3D>

ObliqueSliceWidget::ObliqueSliceWidget(
    std::shared_ptr<Environment> env, std::shared_ptr<SharedProperties> properties,
    QWidget* parent, Qt::WindowFlags f)
    : QOpenGLWidget(parent, f), m_environment(env), m_properties{properties},
    m_cubePlaneIntersection(m_properties->clippingPlane().plane())
{
    m_modelViewMatrix.scale(1/sqrt(3.0));
}

void ObliqueSliceWidget::initializeGL()
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
    auto updateObliqueSlice = [this]() {
        Geometry::instance().allocateObliqueSlice(m_cubePlaneIntersection);
    };
    connect(&m_properties.get()->clippingPlane(), &ClippingPlaneProperties::clippingPlaneChanged,
    [this, updateObliqueSlice](const Plane& plane){
        m_cubePlaneIntersection.changePlane(plane);
        updateObliqueSlice();
        update();
    });
    updateObliqueSlice();

}

void ObliqueSliceWidget::paintGL()
{

    Geometry::instance().allocateObliqueSlice(m_cubePlaneIntersection);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_sliceProgram.bind();

    m_sliceProgram.setUniformValue("modelViewMatrix", m_modelViewMatrix);

    glActiveTexture(GL_TEXTURE0);
    m_sliceProgram.setUniformValue("volumeTexture", 0);
    m_environment->volume()->bind();

    Geometry::instance().bindObliqueSliceVertex();

    int location = m_sliceProgram.attributeLocation("vertexPosition");
    m_sliceProgram.enableAttributeArray(location);
    m_sliceProgram.setAttributeBuffer(location, GL_FLOAT, 0, 2,
                                      sizeof(QVector2D));

    Geometry::instance().bindObliqueSliceTexCoord();
    location = m_sliceProgram.attributeLocation("texCoord");
    m_sliceProgram.enableAttributeArray(location);
    m_sliceProgram.setAttributeBuffer(location, GL_FLOAT, 0, 3,
                                      sizeof(QVector3D));
    Geometry::instance().drawObliqueSlice();

    glActiveTexture(GL_TEXTURE0);
    m_environment->volume()->release();

    m_sliceProgram.release();
}
