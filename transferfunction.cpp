#include "transferfunction.h"

#include "transfertexture.h"
#include <exception> 

namespace tfn
{

TransferFunction::TransferFunction()
{
    m_controlPoints.push_back(points::START_POINT);
    m_controlPoints.push_back(points::END_POINT);
};

bool TransferFunction::addControlPoint(QPointF pos)
{
    if (pos.x() <= tfn::points::START_POINT.x() || pos.x() >= tfn::points::END_POINT.x() || pos.y() <= tfn::points::START_POINT.y() || pos.y() >= tfn::points::END_POINT.y())
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
    m_controlPoints.push_back(tfn::points::START_POINT);
    m_controlPoints.push_back(tfn::points::END_POINT);
};

std::vector<GLfloat>
TransferFunction::applyTransferFunction(const std::vector<GLfloat> cmap)
{
    std::vector<GLfloat> new_cmap{};
    new_cmap.reserve(tfn::size::ARRAY_SIZE);
    int from_x = 0;
    for (int i = 0; i < m_controlPoints.length() - 1; i++)
    {
        auto norm = m_controlPoints[i + 1].x()/tfn::points::END_POINT.x();
        int target_x = static_cast<int>(norm*(tfn::size::NUM_POINTS-1));
        for (int t = from_x; t <= target_x; t++)
        {
            auto r = cmap[t * tfn::size::NUM_CHANNELS];
            auto g = cmap[(t * tfn::size::NUM_CHANNELS) + 1];
            auto b = cmap[(t * tfn::size::NUM_CHANNELS) + 2];
            auto a = getInterpolatedValueBetweenPoints(m_controlPoints[i],
                                       m_controlPoints[i + 1], t);
            new_cmap.push_back(r);
            new_cmap.push_back(g);
            new_cmap.push_back(b);
            new_cmap.push_back(a);
        }
        from_x = target_x + 1;
    }
    assert(new_cmap.size() == tfn::size::ARRAY_SIZE);
    return new_cmap;
}

constexpr float TransferFunction::getInterpolatedValueBetweenPoints(QPointF p0, QPointF p1, int t)
{
    float a = (p1.y() - p0.y());
    float b = (p1.x() - p0.x());
    assert(b > 0 || b < 0);
    float m = a / b;
    return  m * (t - p0.x()) + p0.y();
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
