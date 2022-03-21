#include "clippingplaneproperties.h"

ClippingPlaneProperties::ClippingPlaneProperties(Plane clippingPlane)
    : m_clippingPlane{clippingPlane}
{
}

void ClippingPlaneProperties::updateClippingPlane(Plane clippingPlane)
{
    m_clippingPlane = clippingPlane;
    emit clippingPlaneChanged(clippingPlane);
}
