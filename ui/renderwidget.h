#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include "../environment.h"
#include "../properties/sharedproperties.h"
#include "../properties/clippingplaneproperties.h"
#include "../properties/gradientproperties.h"

#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>
#include <memory>


class RenderWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
    Q_OBJECT

  public:
    RenderWidget(Environment* env, std::shared_ptr<SharedProperties> properties, QWidget* parent = nullptr,
                 Qt::WindowFlags f = Qt::WindowFlags());

  protected:
    virtual void mousePressEvent(QMouseEvent* p_event);
    virtual void mouseMoveEvent(QMouseEvent* p_event);
    virtual void wheelEvent(QWheelEvent* p_event);
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    QVector3D arcballVector(qreal x, qreal y);

  private:
    void updateModelViewMatrix();
    void updateBoxScalingMatrix();

    Environment* m_environment;
    std::shared_ptr<SharedProperties> m_properties;
    QOpenGLShaderProgram m_cubeProgram;
    QMatrix4x4 m_projectionMatrix;
    QMatrix4x4 m_modelViewMatrix;
    QMatrix4x4 m_boxScalingMatrix;
    qreal m_currentX, m_currentY;
    qreal m_previousX, m_previousY;

    qreal m_nearPlane = 0.5;
    qreal m_farPlane = 32.0;
    qreal m_fov = 60.0;
};

#endif
