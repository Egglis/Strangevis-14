#include "gradientproperties.h"

GradientProperties::GradientProperties(GradientMethod gradientMethod) : m_gradientMethod{gradientMethod}
{}

void GradientProperties::updateGradientMethod(GradientMethod gradientMethod)
{
    m_gradientMethod = gradientMethod;
    emit gradientMethodChanged(gradientMethod);
};
