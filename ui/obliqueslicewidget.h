#ifndef OBLIQUESLICEWIDGET_H
#define OBLIQUESLICEWIDGET_H

#include "../environment.h"
#include "../properties/sharedproperties.h"
#include "../properties/clippingplaneproperties.h"
#include "../cubeplaneintersection.h"
#include "parameterwidget.h"

#include <QHBoxLayout>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>


class ObliqueSliceWidget : public QOpenGLWidget,
                              protected QOpenGLExtraFunctions
{
    Q_OBJECT

  public:
    ObliqueSliceWidget(std::shared_ptr<Environment> env,
                          std::shared_ptr<SharedProperties> properties,
                          QWidget* parent = nullptr,
                          Qt::WindowFlags f = Qt::WindowFlags());

  protected:
    virtual void initializeGL();
    virtual void paintGL();

  private:
    std::shared_ptr<SharedProperties> m_properties;
    std::shared_ptr<Environment> m_environment;
    QOpenGLShaderProgram m_sliceProgram;
    CubePlaneIntersection m_cubePlaneIntersection;
    QMatrix4x4 m_modelViewMatrix;

};

#endif // OBLIQUESLICEWIDGET_H
