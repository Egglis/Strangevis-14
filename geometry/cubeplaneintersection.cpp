#include "cubeplaneintersection.h"

#include "utils.h"

#include <QMatrix4x4>
#include <QVector>

CubePlaneIntersection::CubePlaneIntersection(Plane plane, Box box)
    : m_box{box}, m_plane{plane}, m_updateNeeded{true}
{
}

void CubePlaneIntersection::changePlane(Plane plane)
{
    m_plane = plane;
    m_updateNeeded = true;
}

void CubePlaneIntersection::updateIfNeeded()
{
    if (m_updateNeeded)
    {
        m_textureCoords = textureIntersectionVertices();

        if (m_textureCoords.size() > 2)
        {
            m_vertexPositions = rotateToXYPlane(vertexIntersectionVertices());
            m_sortedOrder = convexHullGiftWrapping(m_vertexPositions);
        }
        else
        {
            m_textureCoords = std::vector<QVector3D>{};
            m_vertexPositions = std::vector<QVector2D>{};
            m_sortedOrder = std::vector<unsigned short>{};
        }
    }
    m_updateNeeded = false;
}

std::vector<QVector3D> CubePlaneIntersection::textureIntersectionVertices()
{
    std::vector<QVector3D> intersectionPoints{};
    for (const auto& edge : m_box.cube().edges())
    {
        if (hasLinePlaneIntersection(edge))
        {
            intersectionPoints.push_back(linePlaneIntersectionPoint(edge));
        }
    }
    return std::move(intersectionPoints);
}
std::vector<QVector3D> CubePlaneIntersection::vertexIntersectionVertices()
{
    std::vector<QVector3D> intersectionPoints{};
    for (const auto& edge : m_box.edges())
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

void CubePlaneIntersection::changeScaling(QVector3D dims)
{
    m_box = Box(dims);
    m_updateNeeded = true;
}
