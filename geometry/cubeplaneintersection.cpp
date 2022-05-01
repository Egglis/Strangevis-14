#include "cubeplaneintersection.h"

#include "utils.h"

#include <QMatrix4x4>
#include <QVector>

template<>
struct std::hash<QVector3D>
{
    std::size_t operator()(QVector3D const& v) const noexcept
    {
        std::size_t h1 = std::hash<float>{}(v.x());
        std::size_t h2 = std::hash<float>{}(v.y());
        std::size_t h3 = std::hash<float>{}(v.z());
        return h1 ^ h2 ^ h3;
    }
};
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
        auto intersections = getCubeIntersections();
        auto rotationMatrix =
            rotateToXYPlaneRotationMatrix(intersections);
        m_modelRotationMatrix = rotationMatrix;
        m_sortedOrder = convexHullGiftWrapping(intersections);
    }
    else
    {
        m_cubeIntersections = {};
        m_modelRotationMatrix = {};
        m_sortedOrder = {};
    }
}

QSet<QVector3D> CubePlaneIntersection::cubeIntersectionVertices()
{
    QSet<QVector3D> intersectionPoints{};
    for (const auto& edge : m_cube.edges())
    {
        switch (hasLinePlaneIntersection(edge))
        {
        case Intersection::SINGLE:
        {
            intersectionPoints.insert(linePlaneIntersectionPoint(edge));
            break;
        }
        case Intersection::PARALLEL:
        {
            intersectionPoints.insert(edge.start());
            intersectionPoints.insert(edge.end());
            break;
        }
        default:
            break;
        }
    }
    return intersectionPoints;
}

bool CubePlaneIntersection::hasEdgeParallelToPlane(Edge e) const {
    return m_plane.pointInPlane(e.start()) && m_plane.pointInPlane(e.end());
}
Intersection CubePlaneIntersection::hasLinePlaneIntersection(Edge e) const
{
    // Looks for intersection at ray cast from start towards end, and
    // from end towards start. If both have intersection, plane is
    // between them and the intersection is on the line segment.
    auto singleIntersection =
        hasRayPlaneIntersection(e.start(), e.direction()) &&
        hasRayPlaneIntersection(e.end(), -e.direction());
    auto parallel = hasEdgeParallelToPlane(e);
    if (singleIntersection)
        return Intersection::SINGLE;
    if (parallel)
        return Intersection::PARALLEL;
    return Intersection::NONE;
}

bool CubePlaneIntersection::hasRayPlaneIntersection(QVector3D rayOrigin,
                                                    QVector3D rayDirection) const
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
    if (std::abs(denominator) < 0.00001)
        return e.parameterization(0);
    double t =
        QVector3D::dotProduct(m_plane.point() - e.start(), m_plane.normal()) /
        denominator;
    return e.parameterization(t);
}
