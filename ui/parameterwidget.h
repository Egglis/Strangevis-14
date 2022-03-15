
#ifndef PARAMETER_WIDGET_H
#define PARAMETER_WIDGET_H

#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QWidget>
#include <memory>


class SharedProperties;
class GradientMethodWidget;

class ParameterWidget : public QWidget
{
    Q_OBJECT
  public:
    ParameterWidget(const std::shared_ptr<SharedProperties>& properties,
                    QWidget* parent);
    QSize sizeHint() { return QSize{300, 200}; };

  private:
    const std::shared_ptr<SharedProperties>& m_properties;
    QSlider* createClippingPlaneSlider();
    void createSliderLabel(QSlider* slider);
    QVBoxLayout layout;
    QVector<QSlider*> clippingPlaneSliders;
    QVector<QLabel*> sliderLabels;

    GradientMethodWidget* m_gradientMethodWidget;

    float m_lowerBound = -2.0f;
    float m_upperBound = 2.0f;
    int m_sliderMaximum = 100;
    int m_sliderMinimum = 0;

    float intToFloat(int value);
    int floatToInt(float value);
  private slots:
    void updateClippingPlane();
};

class GradientMethodWidget : public QWidget
{
    Q_OBJECT
    public:
    GradientMethodWidget(QWidget* parent);
    public slots:
    void setValue(int value);
    private slots:
    void updateLabel(int value);
    signals:
    void valueChanged(int value);

  private:
    QSlider* m_gradientMethodSlider;
    QLabel* m_gradientMethodLabel;
    QFormLayout m_layout;
};

#endif
