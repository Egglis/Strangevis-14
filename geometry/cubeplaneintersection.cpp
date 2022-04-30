#include "cubeplaneintersection.h"

#include "utils.h"

#include <QMatrix4x4>
#include <QVector>

CubePlaneIntersection::CubePlaneIntersection(Plane plane)
    : m_cube{}, m_plane{plane}
{
    updateIntersections();
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
        auto rotationMatrix =
            rotateToXYPlaneRotationMatrix(m_cubeIntersections);
        m_modelRotationMatrix = rotationMatrix;
        m_sortedOrder = convexHullGiftWrapping(m_cubeIntersections);
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
    if (hasEdgeParallelToPlane(intersectionPoints))
    {
        /* Alternative Algorithm */
    }
    return intersectionPoints;
}

bool CubePlaneIntersection::hasEdgeParallelToPlane(
    std::vector<QVector3D> intersections)
{
    if (intersections.size() > 0 && intersections.size() < 3)
        return true;
    std::sort(intersections.begin(), intersections.end(), edgeLessThan);
    if (std::adjacent_find(intersections.begin(), intersections.end()) !=
        intersections.end())
        return true;
    return false;
}
bool CubePlaneIntersection::hasLinePlaneIntersection(Edge e)
{
    // Looks for intersection at ray cast from start towards end, and
    // from end towards start. If both have intersection, plane is
    // between them and the intersection is on the line segment.
    static int i = 0;
    auto firstTest = hasRayPlaneIntersection(e.start(), e.direction());
    auto secondTest = hasRayPlaneIntersection(e.end(), -e.direction());
    if (m_plane.pointInPlane(e.start()))
        qDebug() << "Start point in Plane" << i;
    if (m_plane.pointInPlane(e.end()))
        qDebug() << "End point in Plane" << i;
    i++;
    if (i == 10000)
        i = 0;
    return firstTest && secondTest;
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
    bool ret = m_plane.pointInPlane(rayOrigin);
    if (ret)
        qDebug() << "RayOrigin in Plane";
    return ret;
}

QVector3D CubePlaneIntersection::linePlaneIntersectionPoint(Edge e) const
{
    double denominator = QVector3D::dotProduct(m_plane.normal(), e.direction());
    if (std::abs(denominator) < 0.00001)
        return e.parameterization(0);
    double t =
        QVector3D::dotProduct(m_plane.point() - e.start(), m_plane.normal()) /
        denominator;
    return e.parameterization(t);
}
