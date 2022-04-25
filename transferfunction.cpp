#include "transferfunction.h"

#include "transfertexture.h"

namespace tfn
{

TransferFunction::TransferFunction() { reset(); };

QList<QPointF> TransferFunction::getSeriesPoints()
{
    QList<QPointF> seriesPoints;
    for (ControlPoint cp : m_controlPoints)
    {
        seriesPoints.append(QPointF(cp.x(), cp.y()));
    }
    return seriesPoints;
};

// TODO Should update both neigbour points and link their spline settings?
bool TransferFunction::addControlPoint(ControlPoint cp)
{
    if (cp.x() <= tfn::points::START_POINT.x() ||
        cp.x() >= tfn::points::END_POINT.x() ||
        cp.y() <= tfn::points::START_POINT.y() ||
        cp.y() >= tfn::points::END_POINT.y())
    {
        return false;
    }
    for (int i = 0; i < m_controlPoints.length(); i++)
    {
        if (m_controlPoints[i].x() < cp.x() &&
            cp.x() < m_controlPoints[i + 1].x())
        {
            m_controlPoints.insert(i + 1, cp);
            break;
        }
    }
    return true;
};

// TODO Should remove and re check the controll points settings after deletion
bool TransferFunction::removeControlPoint(ControlPoint cp)
{
    auto index = m_controlPoints.indexOf(cp);
    if (index != 0 && index != m_controlPoints.size() - 1)
    {
        m_controlPoints.removeAll(cp);
        return true;
    }
    return false;
}

void TransferFunction::reset()
{
    m_controlPoints.clear();
    m_controlPoints.push_back(ControlPoint(tfn::points::START_POINT));
    m_controlPoints.push_back(ControlPoint(tfn::points::END_POINT));
};

std::vector<GLfloat>
TransferFunction::applyTransferFunction(const std::vector<GLfloat> cmap)
{
    std::vector<GLfloat> new_cmap{};
    new_cmap.reserve(tfn::size::ARRAY_SIZE);
    int from_x = 0;
    for (int i = 0; i < m_controlPoints.length() - 1; i++)
    {
        auto norm = m_controlPoints[i + 1].x() / tfn::points::END_POINT.x();
        int target_x = static_cast<int>(norm * (tfn::size::NUM_POINTS - 1));
        for (int t = from_x; t <= target_x; t++)
        {
            auto r = cmap[t * tfn::size::NUM_CHANNELS];
            auto g = cmap[(t * tfn::size::NUM_CHANNELS) + 1];
            auto b = cmap[(t * tfn::size::NUM_CHANNELS) + 2];
            auto a = getInterpolatedValueBetweenPoints(
                m_controlPoints.at(i), m_controlPoints.at(i + 1),
                t * (tfn::points::END_POINT.x() / static_cast<float>(255)));
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

// TODO De Casteljau
constexpr float TransferFunction::getInterpolatedValueBetweenPoints(QPointF p0,
                                                                    QPointF p1,
                                                                    int t)
{
    if (p0.x() == p1.x())
    {
        return qMax(p0.y(), p1.y());
    }
    float a = (p1.y() - p0.y());
    float b = (p1.x() - p0.x());
    assert(b > 0 || b < 0);
    float m = a / b;
    return m * (t - p0.x()) + p0.y();
};

// TODO re-do settings when replacing as-well
void TransferFunction::replace(int index, ControlPoint cp)
{
    if (index == 0 || index == m_controlPoints.size() - 1)
    {
        cp = QPointF(m_controlPoints.at(index).x(),
                     qMax(qMin(tfn::points::END_POINT.y(), cp.y()),
                          tfn::points::START_POINT.y()));
        m_controlPoints.replace(index, cp);
    }
    else
    {
        m_controlPoints.replace(index, clampToDomain(index, cp));
    }
};

QPointF TransferFunction::clampToDomain(int index, ControlPoint cp)
{
    const QPointF max = tfn::points::END_POINT;
    const QPointF min = tfn::points::START_POINT;
    const QPointF left = m_controlPoints.at(index - 1);
    const QPointF right = m_controlPoints.at(index + 1);
    return QPointF(qMax(qMin(right.x(), cp.x()), left.x()),
                   qMax(qMin(max.y(), cp.y()), min.y()));
};

// TODO seperate functions for manipulating controll points settings here!
ControlPoint::ControlPoint(QPointF position)
{
    qDebug() << "New constorll point at" << position;
    this->setX(position.x());
    this->setY(position.y());
    for (int i = 0; i < 4; i++)
    {
        m_controlNodes.append(QPointF(0, 0));
    }
};

void ControlPoint::setControlNode(int index, QPointF pos)
{
    m_controlNodes.replace(index, pos);
};

} // namespace tfn
