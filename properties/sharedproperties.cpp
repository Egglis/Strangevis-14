#include "sharedproperties.h"

SharedProperties::SharedProperties()
    : m_clippingPlane{-1, 0, 0, 1000}, m_gradientMethod{0}
{
}
void SharedProperties::updateClippingPlane(QVector4D clippingPlane)
{
    m_clippingPlane = clippingPlane;
    emit clippingPlaneChanged(clippingPlane);
};
void SharedProperties::updateGradientMethod(int gradientMethod)
{
    m_gradientMethod = gradientMethod;
    emit gradientMethodChanged(gradientMethod);
};
