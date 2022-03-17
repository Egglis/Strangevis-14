#ifndef SHAREDPROPERTIES_H
#define SHAREDPROPERTIES_H

#include "slicingplaneproperties.h"

#include <QObject>
#include <QVector3D>

class SharedProperties : public QObject
{
    Q_OBJECT
  public:
    SharedProperties();

    const QVector4D& clippingPlane() { return m_clippingPlane; };
    void updateClippingPlane(QVector4D clippingPlane);

    const int gradientMethod() { return m_gradientMethod; };
    SlicingPlaneProperties& slicingPlane() { return m_slicingPlane; }
  public slots:
    void updateGradientMethod(int gradientMethod);

  signals:
    void clippingPlaneChanged(const QVector4D& newClippingPlane);
    void gradientMethodChanged(int gradientMethod);

  private:
    SlicingPlaneProperties m_slicingPlane;
    QVector4D m_clippingPlane;
    int m_gradientMethod;
};

#endif // SHAREDPROPERTIES_H
