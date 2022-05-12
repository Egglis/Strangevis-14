#include "clippingplaneproperties.h"
#include <QDebug>

ClippingPlaneProperties::ClippingPlaneProperties(Plane clippingPlane)
    : m_clippingPlane{clippingPlane}
{
}

void ClippingPlaneProperties::updateClippingPlane(Plane clippingPlane)
{
    qDebug() << "ClippingPlane Updated";
    m_clippingPlane = clippingPlane;
    emit clippingPlaneChanged(clippingPlane);
}

void ClippingPlaneProperties::reset()
{
    updateClippingPlane(Plane{});
}

void ClippingPlaneProperties::updateSelectedPoint(QVector3D point)
{
    m_selectedPoint = point;
    emit selectedPointChanged(point);
}
