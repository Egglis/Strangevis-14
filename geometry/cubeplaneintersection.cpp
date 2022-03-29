#include "cubeplaneintersection.h"

#include "utils.h"

#include <QMatrix4x4>
#include <QVector>

CubePlaneIntersection::CubePlaneIntersection(Plane plane)
    : m_cube{}, m_plane{plane}
{
}

void CubePlaneIntersection::changePlane(Plane plane)
{
    m_plane = plane;
    updateIntersections();
}

void CubePlaneIntersection::updateIntersections()
{
    m_cubeIntersections = cubeIntersectionVertices();

    if (m_cubeIntersections.size() > 2)
    {
        auto [rotationMatrix, vertexPositions] =
            rotateToXYPlane(m_cubeIntersections);
        m_modelRotationMatrix = rotationMatrix;
        m_sortedOrder = convexHullGiftWrapping(m_cubeIntersections, rotationMatrix);
    }
    else
    {
        m_cubeIntersections = {};
        m_modelRotationMatrix = {};
        m_sortedOrder = {};
    }
}

std::vector<QVector3D> CubePlaneIntersection::cubeIntersectionVertices()
{
    std::vector<QVector3D> intersectionPoints{};
    for (const auto& edge : m_cube.edges())
    {
        if (hasLinePlaneIntersection(edge))
        {
            intersectionPoints.push_back(linePlaneIntersectionPoint(edge));
        }
    }
    return std::move(intersectionPoints);
}

bool CubePlaneIntersection::hasLinePlaneIntersection(Edge e)
{
    // Looks for intersection at ray cast from start towards end, and
    // from end towards start. If both have intersection, plane is
    // between them and the intersection is on the line segment.
    return (hasRayPlaneIntersection(e.start(), e.direction()) &&
            hasRayPlaneIntersection(e.end(), -e.direction()));
}

bool CubePlaneIntersection::hasRayPlaneIntersection(QVector3D rayOrigin,
                                                    QVector3D rayDirection)
{
    double epsilon = 0.000001;
    double denominator = QVector3D::dotProduct(m_plane.normal(), rayDirection);
    if (std::abs(denominator) > epsilon)
    {
        double t = QVector3D::dotProduct(m_plane.point() - rayOrigin,
                                         m_plane.normal()) /
                   denominator;
        return (t > epsilon);
    }
    return false;
}

QVector3D CubePlaneIntersection::linePlaneIntersectionPoint(Edge e) const
{
    double denominator = QVector3D::dotProduct(m_plane.normal(), e.direction());
    double t =
        QVector3D::dotProduct(m_plane.point() - e.start(), m_plane.normal()) /
        denominator;
    return e.parameterization(t);
}
