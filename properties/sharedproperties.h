#ifndef SHAREDPROPERTIES_H
#define SHAREDPROPERTIES_H

#include "clippingplaneproperties.h"
#include "gradientproperties.h"
#include "transferproperties.h"

#include <QObject>
#include <QVector3D>

class SharedProperties : public QObject
{
    Q_OBJECT
  public:
    SharedProperties();

    ClippingPlaneProperties& clippingPlane() { return m_clippingPlane; };
    GradientProperties& gradientMethod() { return m_gradientMethod; };
    TransferProperties& colorMap() {return m_transferFunction; };

  private:
    ClippingPlaneProperties m_clippingPlane;
    GradientProperties m_gradientMethod;
    TransferProperties m_transferFunction;
};

#endif // SHAREDPROPERTIES_H
