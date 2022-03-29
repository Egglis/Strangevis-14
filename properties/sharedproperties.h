#ifndef SHAREDPROPERTIES_H
#define SHAREDPROPERTIES_H

#include "clippingplaneproperties.h"
#include "gradientproperties.h"
#include "transferproperties.h"

#include <QObject>
#include <QVector3D>

class ISharedProperties
{
  public:
    virtual ~ISharedProperties() {}
    virtual ClippingPlaneProperties& clippingPlane() = 0;
    virtual const ClippingPlaneProperties& clippingPlane() const = 0;

    virtual GradientProperties& gradientMethod()  = 0;
    virtual const GradientProperties& gradientMethod() const = 0;

    virtual tfn::TransferProperties& transferFunction() = 0;
    virtual const tfn::TransferProperties& transferFunction() const = 0;
};
class SharedProperties : public QObject, public ISharedProperties
{
    Q_OBJECT
  public:
    SharedProperties();

    virtual ClippingPlaneProperties& clippingPlane() { return m_clippingPlane; };
    virtual const ClippingPlaneProperties& clippingPlane() const { return m_clippingPlane; };

    virtual GradientProperties& gradientMethod() { return m_gradientMethod; };
    virtual const GradientProperties& gradientMethod() const { return m_gradientMethod; };

    virtual tfn::TransferProperties& transferFunction() {return m_transferFunction; };
    virtual const tfn::TransferProperties& transferFunction() const {return m_transferFunction; };

  private:
    ClippingPlaneProperties m_clippingPlane;
    GradientProperties m_gradientMethod;
    tfn::TransferProperties m_transferFunction;
};

#endif // SHAREDPROPERTIES_H
