#ifndef OBLIQUESLICEWIDGET_H
#define OBLIQUESLICEWIDGET_H

#include "../geometry/cubeplaneintersection.h"
#include "../properties/clippingplaneproperties.h"
#include "../properties/sharedproperties.h"
#include "../texturestore.h"
#include "parameterwidget.h"

#include <QCheckBox>
#include <QDial>
#include <QHBoxLayout>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QStackedLayout>

class ObliqueSliceRenderWidget : public QOpenGLWidget,
                                 protected QOpenGLExtraFunctions
{
    Q_OBJECT

  public:
    ObliqueSliceRenderWidget(const std::shared_ptr<ITextureStore> textureStore,
                             const std::shared_ptr<const ISharedProperties> properties,
                             QWidget* parent = nullptr,
                             Qt::WindowFlags f = Qt::WindowFlags());

  public slots:
    void rotate(float degrees);
    void flipHorizontal(bool flip);
    void flipVertical(bool flip);

  protected:
    virtual void initializeGL();
    virtual void paintGL();

  private:
    void updateTransferTexture(ColorMap cmap);
    const std::shared_ptr<ITextureStore> m_textureStore;
    const std::shared_ptr<const ISharedProperties> m_properties;
    QOpenGLShaderProgram m_sliceProgram;
    CubePlaneIntersection m_cubePlaneIntersection;
    QMatrix4x4 m_modelViewMatrix;
    float m_prevRotation;
    bool m_horizontalFlipped;
    bool m_verticalFlipped;
};

class ObliqueSliceWidget : public QWidget
{
    Q_OBJECT
  public:
    ObliqueSliceWidget(const std::shared_ptr<ITextureStore>& textureStore,
                       const std::shared_ptr<ISharedProperties>& properties,
                       QWidget* parent = nullptr,
                       Qt::WindowFlags f = Qt::WindowFlags());

    ObliqueSliceRenderWidget* renderWidget() { return m_renderWidget; };

  private:
    ObliqueSliceRenderWidget* m_renderWidget;
    QStackedLayout* m_layout;
    QDial* m_dial;
};

class ObliqueSliceRotationWidget : public QWidget
{
    Q_OBJECT
  public:
    ObliqueSliceRotationWidget(
        const std::shared_ptr<ISharedProperties>& properties,
        QWidget* parent = nullptr);

  signals:
    void angleChanged(float angleDegrees);
    void flipVertical(bool flip);
    void flipHorizontal(bool flip);

  private:
    QDial m_dial;
    QCheckBox m_flipHorizontalCheckbox;
    QCheckBox m_flipVerticalCheckbox;
    QHBoxLayout m_layout;
    ParameterWidget m_parameterWidget;
};
#endif // OBLIQUESLICEWIDGET_H
