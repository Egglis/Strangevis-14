#ifndef CLIPPINGPLANEPROPERTIES_H
#define CLIPPINGPLANEPROPERTIES_H

#include "../geometry/plane.h"

#include <QObject>

class ClippingPlaneProperties : public QObject
{
    Q_OBJECT
  public:
    ClippingPlaneProperties(Plane clippingPlane);
    const Plane& plane() const { return m_clippingPlane; };
  public slots:
    void updateClippingPlane(Plane clippingPlane);

  signals:
    void clippingPlaneChanged(Plane clippingPlane);

  private:
    Plane m_clippingPlane;
};

#endif // CLIPPINGPLANEPROPERTIES_H
