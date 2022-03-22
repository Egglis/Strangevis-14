#ifndef UTILS_H
#define UTILS_H

#include "plane.h"

#include <QVector3D>
#include <vector>

std::vector<QVector2D> rotateToXYPlane(const std::vector<QVector3D>& input);

#endif // UTILS_H
