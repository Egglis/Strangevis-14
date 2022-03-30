#include "transferfunction.h"

#include "transfertexture.h"

namespace tfn
{

TransferFunction::TransferFunction() : m_type("LINEAR")
{
    m_controlPoints.push_back(QPointF(0, 0));
    m_controlPoints.push_back(QPointF(255, 1));
};

bool TransferFunction::addControlPoint(QPointF pos)
{
    if (pos.x() <= 0 || pos.x() >= 255 || pos.y() <= 0 || pos.y() >= 1)
    {
        return false;
    }
    for (int i = 0; i < m_controlPoints.length(); i++)
    {
        if (m_controlPoints[i].x() < pos.x() &&
            pos.x() < m_controlPoints[i + 1].x())
        {
            m_controlPoints.insert(i + 1, pos);
            break;
        }
    }
    return true;
};

void TransferFunction::reset()
{
    m_controlPoints.clear();
    m_controlPoints.push_back(QPointF(0, 0));
    m_controlPoints.push_back(QPointF(255, 1));
};

/*
Applies the transfer function on colormap: cmap
*/
std::vector<GLfloat>
TransferFunction::applyTransferFunction(std::vector<GLfloat> cmap)
{
    std::vector<GLfloat> new_cmap{};
    new_cmap.reserve(size::ArraySize);

    int from_x = 0; // From x
    for (int i = 0; i < m_controlPoints.length() - 1; i++)
    {
        int target_x = (int)m_controlPoints[i + 1].x(); // Target x
        for (int t = from_x; t <= target_x; t++)
        {
            auto r = cmap[t * size::NumChannels];
            auto g = cmap[(t * size::NumChannels) + 1];
            auto b = cmap[(t * size::NumChannels) + 2];
            auto a = solveLineEquation(m_controlPoints[i],
                                       m_controlPoints[i + 1], t);
            new_cmap.push_back(r);
            new_cmap.push_back(g);
            new_cmap.push_back(b);
            new_cmap.push_back(a);
        }
        from_x = target_x + 1;
    }
    assert(new_cmap.size() == size::ArraySize);
    return new_cmap;
}

float TransferFunction::solveLineEquation(QPointF p0, QPointF p1, int t)
{
    float m = (p1.y() - p0.y()) / (p1.x() - p0.x());
    return m * (t - p0.x()) + p0.y();
};

int TransferFunction::replace(int index, QPointF point)
{
    // If replacing first and last points reordering is not necassary
    if (index == 0 || index == m_controlPoints.size() - 1 ||
        m_controlPoints.size() - 1 == 2)
    {
        m_controlPoints.replace(index, point);
        return index;
    }
    else
    {
        // Checks if reordering is necassary and sorts controllpoints and returns new index
        QPointF n0 = m_controlPoints[index - 1];
        QPointF n1 = m_controlPoints[index + 1];
        if (n0.x() > point.x() || n1.x() < point.x()) 
        {
            m_controlPoints.replace(index, point);
            std::sort(
                m_controlPoints.begin(), m_controlPoints.end(),
                [](const auto& x1, const auto& x2) { return x1.x() < x2.x(); });
            return m_controlPoints.indexOf(point);
        }
        else
        {
            m_controlPoints.replace(index, point);
            return index;
        }
    }
};

} // namespace tfn
