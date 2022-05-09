#ifndef RENDERSETTINGSWIDGET_H
#define RENDERSETTINGSWIDGET_H

#include "../properties/sharedproperties.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMap>
#include <QRadioButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>

class SliderWidget : public QWidget
{
    Q_OBJECT
  public:
    SliderWidget(QWidget* parent);
    void setTickInterval(int td) { m_tickInterval = td; };
    void setSliderBounds(int min, int max);
    QString getName() { return m_varLabel->text(); };

    QLabel* m_varLabel;
    QLabel* m_valLabel;
    QSlider* m_slider;

    QHBoxLayout m_layout;

    int m_tickInterval = 1;
    int m_sliderMaximum = 100;
    int m_sliderMinimum = 0;
  signals:
    void valueChanged(int value);
};

class FloatSlider : public SliderWidget
{
    Q_OBJECT
  public:
    FloatSlider(QString name, float value, QWidget* parent = nullptr);
    void setBounds(float min, float max);
    float getValue();
  public slots:
    void setValue(float value);
  private:
    float intToFloat(int value);
    int floatToInt(float value);

    float m_lowerBound = 0.0f;
    float m_upperBound = 10.0f;
};

class BoolCheckbox : public QWidget
{
    Q_OBJECT
  public:
    BoolCheckbox(QString name, bool value, QWidget* parent = nullptr);
    bool getValue() { return m_radioButton->isChecked(); };
    QString getName() { return m_varLabel->text(); };
  public slots:
    void setValue(bool value) { m_radioButton->setChecked(value); };
  signals:
    void valueChanged(bool value);

  private:
    QLabel* m_varLabel;
    QRadioButton* m_radioButton;
    QHBoxLayout m_layout;
};

class RenderSettingsWidget : public QWidget
{
    Q_OBJECT
  public:
    RenderSettingsWidget(const std::shared_ptr<ISharedProperties> properties,
                         QWidget* parent = nullptr);
  public slots:
    void updateRenderSettings();

  private:
    void setupWidgets();
    const std::shared_ptr<ISharedProperties> m_properties;

    QVBoxLayout* m_layout;
    RenderSettings m_renderSettings;
    QMap<QString, FloatSlider*> m_floatSliders;
    QMap<QString, BoolCheckbox*> m_boolCheckboxes;
};

#endif