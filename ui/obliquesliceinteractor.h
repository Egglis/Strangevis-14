#ifndef OBLIQUESLICEINTERACTOR_H
#define OBLIQUESLICEINTERACTOR_H

#include "../renderers/obliqueslicewidget.h"
#include "parameterwidget.h"

#include <QCheckBox>
#include <QDial>
#include <QHBoxLayout>
#include <QWheelEvent>

class ObliqueSliceInteractor : public ObliqueSliceRenderWidget
{
    Q_OBJECT
  public:
    ObliqueSliceInteractor(const std::shared_ptr<ITextureStore> textureStore,
                           const std::shared_ptr<ISharedProperties> properties,
                           QWidget* parent = nullptr,
                           Qt::WindowFlags f = Qt::WindowFlags());

  protected:
    virtual void wheelEvent(QWheelEvent* event);
    virtual void resizeEvent(QResizeEvent* event);

  private:
    void placeDial();
    QDial* m_dial;
    const std::shared_ptr<ISharedProperties> m_properties;
    constexpr static int DialSize = 160;
    constexpr static int DialMargin = 16;
};

class ObliqueSliceRotationWidget : public QWidget
{
    Q_OBJECT
  public:
    ObliqueSliceRotationWidget(
        const std::shared_ptr<ISharedProperties>& properties,
        QWidget* parent = nullptr);

  signals:
    void flipVertical(bool flip);
    void flipHorizontal(bool flip);

  private:
    QCheckBox m_flipHorizontalCheckbox;
    QCheckBox m_flipVerticalCheckbox;
    QHBoxLayout m_layout;
    ParameterWidget m_parameterWidget;
};

#endif // OBLIQUESLICEINTERACTOR_H
