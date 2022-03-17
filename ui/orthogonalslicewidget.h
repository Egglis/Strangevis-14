#ifndef SLICERENDERERWIDGET_H
#define SLICERENDERERWIDGET_H

#include "../environment.h"
#include "../properties/sharedproperties.h"

#include <QHBoxLayout>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <memory>


class OrthogonalSliceWidget : public QOpenGLWidget,
                              protected QOpenGLExtraFunctions
{
    Q_OBJECT

  public:
    OrthogonalSliceWidget(Environment* env,
                          std::shared_ptr<SharedProperties> properties,
                          QWidget* parent = nullptr,
                          Qt::WindowFlags f = Qt::WindowFlags());

  protected:
    virtual void wheelEvent(QWheelEvent* p_event);
    virtual void initializeGL();
    virtual void paintGL();

  private:
    void draw();
    void updateModelViewMatrix();
    std::shared_ptr<SharedProperties> x_properties;
    Environment* m_environment;
    QOpenGLShaderProgram m_sliceProgram;
    QMatrix4x4 m_modelViewMatrix;
    int m_slicingPlane;
    float m_slice;
    bool m_flipHorizontal = false;
    bool m_flipVertical = false;
};

class OrthogonalSliceToolbar : public QWidget
{
    Q_OBJECT
  public:
    OrthogonalSliceToolbar(std::shared_ptr<SharedProperties> properties,
                           QWidget* parent = nullptr);

  private:
    std::shared_ptr<SharedProperties> x_properties;
    QVBoxLayout m_layout;
    QHBoxLayout m_planeSelectionLayout;
    QHBoxLayout m_flipSelectionLayout;
};

#endif // SLICERENDERERWIDGET_H
