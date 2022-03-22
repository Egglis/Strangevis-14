#include "sharedproperties.h"

SharedProperties::SharedProperties()
    : m_clippingPlane{QVector4D(0, 0, 1, 1000)}, m_gradientMethod{GradientMethod::CentralDifference}
{
}
