#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include "../properties/sharedproperties.h"
#include "../renderers/raycastingwidget.h"
#include "../texturestore.h"
#include "parameterwidget.h"
#include "transferfunctionwidget.h"

#include <QMatrix4x4>
#include <QVBoxLayout>
#include <memory>

class RayCastingInteractor : public RayCastingWidget
{
    Q_OBJECT

  public:
    RayCastingInteractor(const std::shared_ptr<ITextureStore> textureStore,
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
    qreal m_currentX, m_currentY;
    qreal m_previousX, m_previousY;
};

class ExtendedParameterWidget : public QWidget
{
  public:
    ExtendedParameterWidget(
        const std::shared_ptr<ISharedProperties>& properties,
        const std::shared_ptr<const tfn::IColorMapStore> colorMapStore,
        QWidget* parent);

  private:
    QVBoxLayout m_layout;
    tfn::TransferWidget m_transferWidget;
    ParameterWidget m_parameterWidget;
};
#endif // RENDERWIDGET_H
