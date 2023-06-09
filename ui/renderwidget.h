#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include "../properties/sharedproperties.h"
#include "../properties/cameraproperties.h"
#include "../renderers/raycastingwidget.h"
#include "../renderers/imguizmorenderer.h"
#include "../texturestore.h"
#include "transferwidget/transferfunctionwidget.h"

#include <QMatrix4x4>
#include <QTimer>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <memory>


class RayCastingInteractor : public RayCastingWidget
{
    Q_OBJECT

  public:
    RayCastingInteractor(std::unique_ptr<ITextureStore>& textureStore,
                         std::shared_ptr<ISharedProperties> properties,
                         CameraProperties& camera,
                         QWidget* parent = nullptr,
                         Qt::WindowFlags f = Qt::WindowFlags());

    virtual void mousePressEvent(QMouseEvent* p_event);
    virtual void mouseMoveEvent(QMouseEvent* p_event);
    virtual void wheelEvent(QWheelEvent* p_event);
  protected:
    QVector3D arcballVector(qreal x, qreal y);

  private:
    void rotateCamera();
    void moveLightSource();
    const std::shared_ptr<ISharedProperties> m_properties;
    QPointF m_currentPosition;
    QPointF m_previousPosition;
};

class ExtendedParameterWidget : public QWidget
{
    Q_OBJECT;

  public:
    ExtendedParameterWidget(
        const std::shared_ptr<ISharedProperties>& properties,
        const std::shared_ptr<const tfn::IColorMapStore> colorMapStore,
        QWidget* parent);
  public slots:
    void histogramChanged(std::vector<float> normalizedHistogramData);

  private:
    QVBoxLayout m_layout;
    tfn::TransferWidget m_transferWidget;
};
class StackedRayCastingWidget : public QWidget
{
    Q_OBJECT
  public:
    StackedRayCastingWidget(std::unique_ptr<ITextureStore>& textureStore,
                            std::shared_ptr<ISharedProperties> properties,
                            QWidget* renderSettings,
                            QWidget* lightSettings,
                            QWidget* parent = nullptr,
                            Qt::WindowFlags f = Qt::WindowFlags());

  protected:
    virtual void mousePressEvent(QMouseEvent* p_event);
    virtual void mouseMoveEvent(QMouseEvent* p_event);
    virtual void wheelEvent(QWheelEvent* p_event);

  private:
    QStackedLayout* m_layout;
    RayCastingInteractor* m_rayCastingWidget;
    ImguizmoWidget* m_imguizmoWidget;
    CameraProperties m_camera;
};
#endif // RENDERWIDGET_H
