#include "sharedproperties.h"

SharedProperties::SharedProperties()
    : m_clippingPlane{0, 0, 1, 1000}, m_gradientMethod{0}
{
}
void SharedProperties::updateClippingPlane(QVector4D clippingPlane)
{
    QVector3D planeNormal = QVector3D(clippingPlane).normalized();
    float d = clippingPlane.w()/planeNormal.length();
    m_clippingPlane = QVector4D(planeNormal, d);
    emit clippingPlaneChanged(clippingPlane);
};
void SharedProperties::updateGradientMethod(int gradientMethod)
{
    m_gradientMethod = gradientMethod;
    emit gradientMethodChanged(gradientMethod);
};
