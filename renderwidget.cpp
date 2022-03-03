#include "renderwidget.h"
#include "geometry.h"

#include <QMouseEvent>
#include <QtMath>

RenderWidget::RenderWidget(Environment* env, QWidget* parent, Qt::WindowFlags f)
    : QOpenGLWidget(parent, f), m_environment(env) {
    m_modelViewMatrix.setToIdentity();
    m_modelViewMatrix.translate(0.0, 0.0, -2.0 * sqrt(3.0)); //?
}

void RenderWidget::mousePressEvent(QMouseEvent* p_event) {
    m_currentX = static_cast<qreal>(p_event->x());
    m_currentY = static_cast<qreal>(p_event->y());

    m_previousX = m_currentX;
    m_previousY = m_currentY;
}

void RenderWidget::mouseMoveEvent(QMouseEvent* p_event) {
    m_currentX = static_cast<qreal>(p_event->x());
    m_currentY = static_cast<qreal>(p_event->y());

    if (p_event->buttons() & Qt::LeftButton) {
        if (m_currentX != m_previousX || m_currentY != m_previousY) {
            updateModelViewMatrix();
        }
    }
    m_previousX = m_currentX;
    m_previousY = m_currentY;

    update();
}

void RenderWidget::updateModelViewMatrix() {
    QVector3D va = arcballVector(m_previousX, m_previousY);
    QVector3D vb = arcballVector(m_currentX, m_currentY);

    if (va != vb) {
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

void RenderWidget::initializeGL() {
    initializeOpenGLFunctions();
    // initialize geometry
    Geometry::instance();

    if (!m_cubeProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                               ":/shaders/cube-vs.glsl"))
        qDebug() << "Could not load vertex shader!";

    if (!m_cubeProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                               ":/shaders/cube-fs.glsl"))
        qDebug() << "Could not load fragment shader!";

    if (!m_cubeProgram.link())
        qDebug() << "Could not link shader program!";
}

void RenderWidget::resizeGL(int w, int h) {
    qreal aspectRatio = static_cast<qreal>(w) / static_cast<qreal>(h);

    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(m_fov, aspectRatio, m_nearPlane, m_farPlane);
}

void RenderWidget::paintGL() {

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    QMatrix4x4 modelViewProjectionMatrix =
        m_projectionMatrix * m_modelViewMatrix;

    m_cubeProgram.bind();
    m_cubeProgram.setUniformValue("modelViewProjectionMatrix",
                                  modelViewProjectionMatrix);

    glActiveTexture(GL_TEXTURE0);
    m_cubeProgram.setUniformValue("volumeTexture", 0);
    m_environment->volume()->bind();

    Geometry::instance().bindCube();

    int location = m_cubeProgram.attributeLocation("vertexPosition");
    m_cubeProgram.enableAttributeArray(location);
    m_cubeProgram.setAttributeBuffer(location, GL_FLOAT, 0, 3,
                                     sizeof(QVector3D));

    Geometry::instance().drawCube();

    glActiveTexture(GL_TEXTURE0);
    m_environment->volume()->release();

    m_cubeProgram.release();
}

QVector3D RenderWidget::arcballVector(qreal x, qreal y) {
    QVector3D p = QVector3D(
        2.0f * static_cast<float>(x) / static_cast<float>(this->width()) - 1.0f,
        -2.0f * static_cast<float>(y) / static_cast<float>(this->height()) +
            1.0f,
        0.0);

    float length2 = p.x() * p.x() + p.y() * p.y();

    if (length2 < 1.0f) {
        p.setZ(sqrtf(1.0f - length2));
    } else {
        p.normalize();
    }
    return p;
}