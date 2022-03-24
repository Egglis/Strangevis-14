#ifndef PARAMETERWIDGET_H
#define PARAMETERWIDGET_H

#include "../properties/gradientproperties.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QWidget>
#include <memory>

class SharedProperties;
class GradientMethodWidget;
class ClippingWidget;

class ParameterWidget : public QWidget
{
    Q_OBJECT
  public:
    ParameterWidget(const std::shared_ptr<SharedProperties>& properties,
                    QWidget* parent);
    QSize sizeHint() { return QSize{300, 200}; };

    const std::shared_ptr<SharedProperties>& m_properties;
  public slots:
    void updateClippingPlane();

  private:
    QHBoxLayout m_layout;
    QVector<ClippingWidget*> m_clippingPlaneWidgets;
    GradientMethodWidget* m_gradientMethodWidget;
};

class ClippingWidget : public QWidget
{
    Q_OBJECT
  public:
    ClippingWidget(QWidget* parent);
    float getValue() { return intToFloat(m_slider->value()); };
  public slots:
    void setValue(float value);
  signals:
    void valueChanged(int value);

  private:
    float intToFloat(int value);
    int floatToInt(float value);

    QFormLayout m_layout;
    QSlider* m_slider;
    QLabel* m_label;

    float m_lowerBound = -2.0f;
    float m_upperBound = 2.0f;
    int m_sliderMaximum = 100;
    int m_sliderMinimum = 0;
};

class GradientMethodWidget : public QWidget
{
    Q_OBJECT
  public:
    GradientMethodWidget(QWidget* parent);
  public slots:
    void setValue(GradientMethod method);
    void setValue(int value);
  private slots:
    void updateLabel(GradientMethod method);
  signals:
    void valueChanged(GradientMethod method);

  private:
    QSlider* m_gradientMethodSlider;
    QLabel* m_gradientMethodLabel;
    QFormLayout m_layout;
};

#endif // PARAMETERWIDGET_H
