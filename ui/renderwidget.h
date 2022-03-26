#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include "../texturestore.h"
#include "../properties/sharedproperties.h"
#include "transferfunctionwidget.h"
#include "parameterwidget.h"

#include <QMatrix4x4>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <memory>


class RenderWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT

  public:
    RenderWidget(const std::shared_ptr<ITextureStore> textureStore,
                 const std::shared_ptr<const ISharedProperties> properties,
                 QWidget* parent = nullptr,
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
    void updateTransferTexture(tfn::ColorMap cmap);

    const std::shared_ptr<ITextureStore> m_textureStore;
    const std::shared_ptr<const ISharedProperties> m_properties;
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

class ExtendedParameterWidget : public QWidget
{
  public:
    ExtendedParameterWidget(const std::shared_ptr<ISharedProperties>& properties,
                            QWidget* parent);

  private:
    QVBoxLayout m_layout;
    tfn::TransferWidget m_transferWidget;
    ParameterWidget m_parameterWidget;
};
#endif // RENDERWIDGET_H
