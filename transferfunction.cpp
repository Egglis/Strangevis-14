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
    m_b.clear();
    for (int i = 0; i < m_controlPoints.length() - 1; i++)
    {
        auto x = m_controlPoints[i + 1].x();
        int target_x = static_cast<int>(x * (tfn::size::NUM_POINTS - 1));
        for (int t = from_x; t <= target_x; t++)
        {
            auto r = cmap[t * tfn::size::NUM_CHANNELS];
            auto g = cmap[(t * tfn::size::NUM_CHANNELS) + 1];
            auto b = cmap[(t * tfn::size::NUM_CHANNELS) + 2];
            QPointF from = m_controlPoints.at(i);
            QPointF target = m_controlPoints.at(i + 1);
            QPointF cp0 = m_controlPoints[i].getControlNodes().at(0);
            QPointF cp1 = m_controlPoints[i].getControlNodes().at(1);
            /*auto a = getInterpolatedValueBetweenPoints(
                m_controlPoints.at(i), m_controlPoints.at(i + 1),
                t * (tfn::points::END_POINT.x() / static_cast<float>(255)));
                */

            // Bezier Test
            auto bt = t / static_cast<float>(tfn::size::NUM_POINTS - 1);
            qDebug() << bt << "T";
            QPointF bez = bezier(
                from, QPointF(cp0.x() - tfn::nodes::OFFSET.x(), cp0.y()),
                QPointF(cp1.x(), cp1.y() - tfn::nodes::OFFSET.y()), target, bt);
            m_b.append(bez);
            // Test end

            auto a = bez.y();
            new_cmap.push_back(r);
            new_cmap.push_back(g);
            new_cmap.push_back(b);
            new_cmap.push_back(a);
            qDebug() << a;
        }
        from_x = target_x + 1;
    }
    assert(new_cmap.size() == tfn::size::ARRAY_SIZE);
    return new_cmap;
}

// TODO De Casteljau
constexpr float TransferFunction::getInterpolatedValueBetweenPoints(QPointF p0,
                                                                    QPointF p1,
                                                                    float t)
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

QPointF TransferFunction::bezier(QPointF p0, QPointF p1, QPointF p2, QPointF p3,
                                 float t)
{
    float xa = getPt(p0.x(), p1.x(), t);
    float ya = getPt(p0.y(), p1.y(), t);
    float xb = getPt(p1.x(), p2.x(), t);
    float yb = getPt(p1.y(), p2.y(), t);
    float xc = getPt(p2.x(), p3.x(), t);
    float yc = getPt(p2.y(), p3.y(), t);

    float xm = getPt(xa, xb, t);
    float ym = getPt(ya, yb, t);
    float xn = getPt(xb, xc, t);
    float yn = getPt(yb, yc, t);

    float x = getPt(xm, xn, t);
    float y = getPt(ym, yn, t);

    return QPointF(x, y);
};

QList<QPointF> TransferFunction::toLineSeries()
{
    for (int i = 0; i < m_controlPoints.length() - 1; i++)
    {
        QPointF from = m_controlPoints.at(i);
        QPointF to = m_controlPoints.at(i + 1);
        QPointF cp0 =
            m_controlPoints[i].getControlNodes().at(tfn::nodes::RIGHT);
        QPointF cp1 = m_controlPoints[i].getControlNodes().at(tfn::nodes::TOP);
    }
};

float TransferFunction::getPt(float n1, float n2, float perc)
{
    float diff = n2 - n1;
    return n1 + (diff * perc);
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
        ControlPoint old_cp = m_controlPoints.at(index);
        ControlPoint new_cp = clampToDomain(cp);
        new_cp.setAllControlNodes(old_cp.getControlNodes());
        m_controlPoints.replace(index, clampToNeighbours(index, clampToDomain(cp)));
    }
};

QPointF TransferFunction::clampToDomain(ControlPoint point)
{
    const QPointF max = tfn::points::END_POINT;
    const QPointF min = tfn::points::START_POINT;
    return QPointF(qMax(qMin(max.x(), point.x()), min.x()),
                   qMax(qMin(max.y(), point.y()), min.y()));
};

QPointF TransferFunction::clampToNeighbours(int index, ControlPoint point)
{
    const QPointF left = m_controlPoints.at(index - 1);
    const QPointF right = m_controlPoints.at(index + 1);
    return QPointF(qMax(qMin(right.x(), point.x()), left.x()), point.y());
};

void TransferFunction::setControlNodePos(int index, int dir, QPointF pos)
{
    m_controlPoints[index].setControlNode(dir, pos);
};

ControlPoint::ControlPoint(QPointF position)
{
    qDebug() << "New constorll point at" << position;
    this->setX(position.x());
    this->setY(position.y());
    m_controlNodes.append(
        QPointF(position.x() + tfn::nodes::OFFSET.x(), position.y()));
    m_controlNodes.append(
        QPointF(position.x(), position.y() + tfn::nodes::OFFSET.y()));
};

void ControlPoint::setControlNode(int index, QPointF pos)
{
    m_controlNodes.replace(index, clampToDirections(index, pos));
};

void ControlPoint::setAllControlNodes(QList<QPointF> nodes)
{
    for (int i = 0; i < 2; i++)
    {
        setControlNode(i, nodes.at(i));
    }
};

QPointF ControlPoint::clampToDirections(int dir, QPointF pos)
{
    switch (dir)
    {
    case 0: // Right
        return QPointF(qMax(pos.x(), x() + tfn::nodes::OFFSET.x()), y());
    case 1: // Top
        return QPointF(x(), qMax(pos.y(), y() + tfn::nodes::OFFSET.y()));
    }
    return pos;
};

} // namespace tfn
