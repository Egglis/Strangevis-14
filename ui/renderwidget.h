#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include "../properties/sharedproperties.h"
#include "../renderers/raycastingwidget.h"
#include "../texturestore.h"
#include "parameterwidget.h"
#include "transferwidget/transferfunctionwidget.h"

#include <QMatrix4x4>
#include <QVBoxLayout>
#include <memory>

class RayCastingInteractor : public RayCastingWidget
{
    Q_OBJECT

  public:
    RayCastingInteractor(std::unique_ptr<ITextureStore>& textureStore,
                         const std::shared_ptr<ISharedProperties> properties,
                         QWidget* parent = nullptr,
                         Qt::WindowFlags f = Qt::WindowFlags());

  protected:
    virtual void mousePressEvent(QMouseEvent* p_event);
    virtual void mouseMoveEvent(QMouseEvent* p_event);
    virtual void wheelEvent(QWheelEvent* p_event);
    QVector3D arcballVector(qreal x, qreal y);

  private:
    void rotateCamera();
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
#endif // RENDERWIDGET_H
