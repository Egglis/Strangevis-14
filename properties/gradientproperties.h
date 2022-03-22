#ifndef GRADIENTPROPERTIES_H
#define GRADIENTPROPERTIES_H

#include <QObject>

enum class GradientMethod {
    CentralDifference = 0,
    ForwardDifference = 1,
    BackwardsDifference = 2
};

class GradientProperties : public QObject
{
    Q_OBJECT
  public:
    GradientProperties(GradientMethod gradientMethod);
    const GradientMethod& method() const { return m_gradientMethod; };
  public slots:
    void updateGradientMethod(GradientMethod gradientMethod);

  signals:
    void gradientMethodChanged(GradientMethod gradientMethod);

  private:
    GradientMethod m_gradientMethod;
};

#endif // GRADIENTPROPERTIES_H
