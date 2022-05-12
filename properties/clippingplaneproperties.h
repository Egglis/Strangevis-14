#ifndef CLIPPINGPLANEPROPERTIES_H
#define CLIPPINGPLANEPROPERTIES_H

#include "../geometry/plane.h"

#include <QObject>

class ClippingPlaneProperties : public QObject
{
    Q_OBJECT
  public:
    ClippingPlaneProperties(){};
    ClippingPlaneProperties(Plane clippingPlane);
    const Plane& plane() const { return m_clippingPlane; };
    const QVector3D& selectedPoint() const { return m_selectedPoint;};
  public slots:
    void updateClippingPlane(Plane clippingPlane);
    void updateSelectedPoint(QVector3D point);
    void reset();

  signals:
    void clippingPlaneChanged(Plane clippingPlane);
    void selectedPointChanged(const QVector3D&);

  private:
    Plane m_clippingPlane;
    QVector3D m_selectedPoint;
};

#endif // CLIPPINGPLANEPROPERTIES_H
