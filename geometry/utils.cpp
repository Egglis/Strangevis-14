#include "utils.h"

#include "plane.h"

#include <QMatrix4x4>

// Adapted from
// https://iq.opengenus.org/gift-wrap-jarvis-march-algorithm-convex-hull/
// Returns the indexes of input that form a convex hull over the points in
// input.
std::vector<unsigned short>
convexHullGiftWrapping(const std::vector<QVector3D>& input)
{

    if (input.size() < 3)
    {
        return {};
    }
    std::vector<QVector2D> rotatedInput = rotateToXYPlane(input);
    std::vector<unsigned short> output{};
    output.reserve(rotatedInput.size());

    int startingVertex = 0;

    // We want to examine all points in input and find the point that forming an
    // edge with our starting point is such that all other points in the input
    // are to the left of the edge.
    enum class Orientation { CounterClockwise, Colinear, Clockwise };
    auto orientation = [](const QVector2D& a, const QVector2D& b,
                          const QVector2D& c) {
        // They are in the xy-plane, so ignore z
        float orient = (b.y() - a.y()) * (c.x() - b.x()) -
                       (c.y() - b.y()) * (b.x() - a.x());
        if (std::abs(orient) < 0.001)
            return Orientation::Colinear;
        return orient < 0 ? Orientation::CounterClockwise
                          : Orientation::Clockwise;
    };
    unsigned short n = static_cast<unsigned short>(rotatedInput.size());
    // Start from any point guaranteed on the hull, keep moving counterclockwise
    // until reach the start point again.  This loop runs O(h)
    // times where h is number of points in result or output.
    unsigned short p = startingVertex;
    unsigned short q;
    do
    {
        output.push_back(p);
        // Search for a point 'q' such that orientation(p, x,
        // q) is counterclockwise for all points 'x'. The idea
        // is to keep track of last visited most counterclock-
        // wise point in q. If any point 'i' is more counterclock-
        // wise than q, then update q.
        q = (p + 1) % n;
        for (unsigned short i = 0; i < n; i++)
        {
            // If i is more counterclockwise than current q, then
            // update q
            if (orientation(rotatedInput[p], rotatedInput[i],
                            rotatedInput[q]) == Orientation::CounterClockwise)
                q = i;
        }
        // Now q is the most counterclockwise with respect to p
        // Set p as q for next iteration, so that q is added to
        // result 'hull'
        p = q;
    } while (p != startingVertex && output.size() < 6); // While we don't come to first point
    return output;
}

QMatrix4x4 rotateToXYPlaneRotationMatrix(const std::vector<QVector3D>& input)
{
    assert(input.size() > 2);
    Plane plane = Plane(input[0], input[1], input[2]);
    QVector3D normal{plane.normal().normalized()};

    // Vector of rotation is orthogonal to plane-normal and (0,0,1)
    QVector3D rotVec = QVector3D::crossProduct(normal, QVector3D(0, 0, 1));

    // Angle of rotation is cos^1(c)
    // QMatrix4x4 uses degrees
    float angle = qRadiansToDegrees(std::acos(normal.z()));

    QMatrix4x4 transformationMatrix{};
    transformationMatrix.translate(-plane.d() * QVector3D(0, 0, 1));
    transformationMatrix.rotate(angle, rotVec);

    return transformationMatrix;
}

std::vector<QVector2D> rotateToXYPlane(const std::vector<QVector3D>& input)
{
    QMatrix4x4 transformationMatrix = rotateToXYPlaneRotationMatrix(input);
    std::vector<QVector2D> vertexPositions{};
    vertexPositions.resize(input.size());

    std::transform(input.begin(), input.end(), vertexPositions.begin(),
                   [&transformationMatrix](QVector3D texCoord) {
                       auto newVertexPosition = transformationMatrix.map(texCoord);
                       assert(std::abs(newVertexPosition.z()) < 0.1);
                       return QVector2D(newVertexPosition);
                   });

    return vertexPositions;
}
