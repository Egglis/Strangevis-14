#ifndef UTILS_H
#define UTILS_H


#include <QVector3D>
#include <vector>

std::vector<unsigned short>
convexHullGiftWrapping(const std::vector<QVector3D>& input);

QMatrix4x4 rotateToXYPlaneRotationMatrix(const std::vector<QVector3D>& input);
std::vector<QVector2D> rotateToXYPlane(const std::vector<QVector3D>& input);

#endif // UTILS_H
