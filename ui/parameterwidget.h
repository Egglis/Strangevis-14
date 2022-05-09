#ifndef PARAMETERWIDGET_H
#define PARAMETERWIDGET_H

#include "../properties/gradientproperties.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QWidget>
#include <memory>

class ISharedProperties;
class GradientMethodWidget;

class ParameterWidget : public QWidget
{
    Q_OBJECT
  public:
    ParameterWidget(const std::shared_ptr<ISharedProperties> properties,
                    QWidget* parent);
    QSize sizeHint() { return QSize{300, 200}; };

  private:
    const std::shared_ptr<ISharedProperties> m_properties;
    QHBoxLayout m_layout;
    GradientMethodWidget* m_gradientMethodWidget;
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
