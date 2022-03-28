#ifndef OBLIQUESLICEWIDGET_H
#define OBLIQUESLICEWIDGET_H

#include "../geometry/cubeplaneintersection.h"
#include "../properties/clippingplaneproperties.h"
#include "../properties/sharedproperties.h"
#include "../texturestore.h"

#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>

class ObliqueSliceRenderWidget : public QOpenGLWidget,
                                 protected QOpenGLExtraFunctions
{
    Q_OBJECT

  public:
    ObliqueSliceRenderWidget(
        const std::shared_ptr<ITextureStore> textureStore,
        const std::shared_ptr<const ISharedProperties> properties,
        QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

  public slots:
    void flipHorizontal(bool flip);
    void flipVertical(bool flip);
  protected slots:
    void rotate(float degrees);

  protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);
    virtual void zoomCamera(float zoomFactor);
  private:
    void correctQuadForAspectRatio(int w, int h);
    void updateTransferTexture(tfn::ColorMap cmap);
    const std::shared_ptr<ITextureStore> m_textureStore;
    const std::shared_ptr<const ISharedProperties> m_properties;
    QOpenGLShaderProgram m_sliceProgram;
    CubePlaneIntersection m_cubePlaneIntersection;
    QMatrix4x4 m_modelViewMatrix;
    QMatrix4x4 m_aspectRatioMatrix;
    float m_prevRotation;
    bool m_horizontalFlipped;
    bool m_verticalFlipped;
};

#endif // OBLIQUESLICEWIDGET_H
