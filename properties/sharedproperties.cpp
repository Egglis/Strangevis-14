#include "sharedproperties.h"

SharedProperties::SharedProperties()
    : m_clippingPlane{QVector4D(0, 0, 1, 0)}, m_gradientMethod{GradientMethod::CentralDifference}, m_transferFunction{}
{
}
