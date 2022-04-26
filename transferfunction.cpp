#include "transferfunction.h"

#include "transfertexture.h"

namespace tfn
{

TransferFunction::TransferFunction()
{
    m_interpolatedPoints.reserve(tfn::size::NUM_POINTS);
    for (int i = 0; i < tfn::size::NUM_POINTS; i++)
    {
        m_interpolatedPoints.append(QPointF(0, 0));
    }
    reset();
    interpolatePoints();
};

void TransferFunction::reset()
{
    m_controlPoints.clear();
    m_controlPoints.push_back(ControlPoint(tfn::points::START_POINT));
    m_controlPoints.push_back(ControlPoint(tfn::points::END_POINT));
};

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

std::vector<GLfloat>
TransferFunction::applyTransferFunction(const std::vector<GLfloat> cmap)
{
    std::vector<GLfloat> new_cmap;
    new_cmap.reserve(tfn::size::NUM_POINTS);
    for (int i = 0; i < m_interpolatedPoints.length(); i++)
    {
        float r = cmap[i * tfn::size::NUM_CHANNELS];
        float g = cmap[i * tfn::size::NUM_CHANNELS + 1];
        float b = cmap[i * tfn::size::NUM_CHANNELS + 2];
        float a = m_interpolatedPoints.at(i).y();

        new_cmap.push_back(r);
        new_cmap.push_back(g);
        new_cmap.push_back(b);
        new_cmap.push_back(a);
    };
    return new_cmap;
}

void TransferFunction::interpolatePoints()
{
    int from_x = 0;
    for (int i = 0; i < m_controlPoints.length() - 1; i++)
    {
        auto x = m_controlPoints[i + 1].x();
        int target_x = static_cast<int>(x * (tfn::size::NUM_POINTS - 1));
        for (int t = from_x; t <= target_x; t++)
        {
            const QPointF from = m_controlPoints.at(i);
            const QPointF target = m_controlPoints.at(i + 1);
            QPointF cp0 = m_controlPoints[i].getControlNodes().at(0);
            QPointF cp1 = m_controlPoints[i].getControlNodes().at(1);

            float perc = (t - from_x) / static_cast<float>(target_x - from_x);
            QPointF interpolatedPoint = deCasteljau(
                from, QPointF(cp0.x() - tfn::nodes::OFFSET.x(), cp0.y()),
                QPointF(cp1.x(), cp1.y() - tfn::nodes::OFFSET.y()), target,
                perc);
            qDebug() << t;
            m_interpolatedPoints.replace(t, interpolatedPoint);
        }
        from_x = target_x + 1;
    }
};

// De Casteljau's algorithm for cubic bezier splines
QPointF TransferFunction::deCasteljau(QPointF p0, QPointF p1, QPointF p2,
                                      QPointF p3, float t)
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

constexpr float TransferFunction::getPt(float n1, float n2, float perc)
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
        // TODO clean-up!!!!
        ControlPoint old_cp = m_controlPoints.at(index);
        ControlPoint new_cp =
            clampToDomain(cp, tfn::points::END_POINT, tfn::points::START_POINT);
        new_cp.setAllControlNodes(old_cp.getControlNodes());
        m_controlPoints.replace(
            index,
            clampToNeighbours(index, clampToDomain(cp, tfn::points::END_POINT,
                                                   tfn::points::START_POINT)));
    }
};

QPointF TransferFunction::clampToDomain(ControlPoint point, QPointF max,
                                        QPointF min)
{
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
    if (index != m_controlPoints.length())
    {
        ControlPoint& cp0 = m_controlPoints[index];
        ControlPoint& cp1 = m_controlPoints[index + 1];
        const QPointF max = QPointF(cp1.x(), qMax(cp0.y(), cp1.y()));
        const QPointF min = QPointF(cp0.x(), qMin(cp0.y(), cp1.y()));
        cp0.setControlNode(dir, clampToDomain(pos, max, min));
    }
};
// TODO disable controll nodes for last index!!
// TODO Make sure settings are not reset when a position is moved
// TODO Makes sure that they are retained if possible and only clamp when needed
// TODO refecator lots of useless code
// TODO removed debugs()
// TODO remove TODOs
// TODO make lineSeries display when using control points
// TODO hide() nodes when moving point appropirately
// TODO maybe move control point into seperate files.cpp/.h
// TODO merge regeast
// TODO get roasted
// TODO make applytransferfunction return a refrences instead of making a copy
// TODO rename some function names in control point to avoid mixup between graph
// TODO add some static namespace VARIBLES for constants!!!

// --------- CONTROL POINT ----------------

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
    m_controlNodes.replace(index, pos);
};

void ControlPoint::setAllControlNodes(QList<QPointF> nodes)
{
    for (int i = 0; i < 2; i++)
    {
        setControlNode(i, nodes.at(i));
    }
};

// TODO remove function
QPointF ControlPoint::clampToDirections(int dir, QPointF pos) { return pos; };

} // namespace tfn
