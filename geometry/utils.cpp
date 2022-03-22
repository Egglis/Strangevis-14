#include "utils.h"
#include "plane.h"

#include <QMatrix4x4>


std::vector<QVector2D>
rotateToXYPlane(const std::vector<QVector3D>& input)
{
    assert(input.size() > 2);
    Plane plane = Plane(input[0], input[1], input[2]);
    QVector3D normal{plane.normal().normalized()};

    std::vector<QVector2D> vertexPositions{input.size()};

    // Vector of rotation is orthogonal to plane-normal and (0,0,1)
    QVector3D rotVec = QVector3D::crossProduct(normal, QVector3D(0, 0, 1));

    // Angle of rotation is cos^1(c)
    // QMatrix4x4 uses degrees
    float angle = qRadiansToDegrees(std::acos(normal.z()));

    QMatrix4x4 tranformationMatrix{};
    tranformationMatrix.translate(-plane.d() * QVector3D(0, 0, 1));
    tranformationMatrix.rotate(angle, rotVec);

    std::transform(input.begin(), input.end(), vertexPositions.begin(),
                   [&tranformationMatrix](QVector3D texCoord) {
                       auto newVertexPosition = tranformationMatrix * texCoord;
                       assert(std::abs(newVertexPosition.z()) < 0.001);
                       return QVector2D(newVertexPosition);
                   });
    return std::move(vertexPositions);
}
