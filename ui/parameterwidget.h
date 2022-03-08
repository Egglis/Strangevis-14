
#ifndef PARAMETER_WIDGET_H
#define PARAMETER_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QSlider>
#include <memory>

class SharedProperties;

class ParameterWidget : public QWidget
{
    Q_OBJECT
    public:
    ParameterWidget(const std::shared_ptr<SharedProperties>& properties, QWidget* parent);
    QSize sizeHint() { return QSize {300, 200};};

    private:
    const std::shared_ptr<SharedProperties>& m_properties;
    QSlider* createSlider();
    QVBoxLayout layout;
    QVector<QSlider*> clippingPlaneSliders;
    QWidget* m_widget;

    float m_lowerBound = -2.0f;
    float m_upperBound = 2.0f;
    int m_sliderMaximum = 100;
    int m_sliderMinimum = 0;

    float intToFloat(int value);
    int floatToInt(float value);
    private slots:
    void updateClippingPlane();


};

#endif
