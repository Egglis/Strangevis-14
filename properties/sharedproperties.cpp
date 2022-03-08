#include "sharedproperties.h"

SharedProperties::SharedProperties() : m_clippingPlane {-1,0,0,1000}
{
}
void SharedProperties::updateClippingPlane(QVector4D clippingPlane)
{
    m_clippingPlane = clippingPlane;
    emit clippingPlaneChanged(clippingPlane);
};
