#include "transferfunction.h"

#include "transfertexture.h"

namespace tfn
{

TransferFunction::TransferFunction()
{
    m_interpolatedPoints = QList<QPointF>(size::NUM_POINTS, QPointF(0, 0));
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
            setControlNodePos(
                i, Nodes::NODE0,
                m_controlPoints[i].getControlNodes().value(Nodes::NODE0));
            setControlNodePos(
                i, Nodes::NODE1,
                m_controlPoints[i].getControlNodes().value(Nodes::NODE1));
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
        m_controlPoints.removeOne(cp);
        return true;
    }
    return false;
}

void TransferFunction::updateTransferFunction()
{
    interpolatePoints();
    applyTransferFunction();
}

void TransferFunction::applyTransferFunction()
{
    for (int i = 0; i < m_interpolatedPoints.length(); i++)
    {
        m_cmapData[(i * size::NUM_CHANNELS) + 3] =
            m_interpolatedPoints.at(i).y();
    };
}

void TransferFunction::setColorMap(ColorMap cmap)
{
    m_cmapData = cmap.colorMapData();
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
            QPointF cp0 =
                m_controlPoints[i].getControlNodes().value(Nodes::NODE0);
            QPointF cp1 =
                m_controlPoints[i].getControlNodes().value(Nodes::NODE1);

            float perc = (t - from_x) / static_cast<float>(target_x - from_x);
            QPointF interpolatedPoint =
                deCasteljau(from, QPointF(cp0.x(), cp0.y()),
                            QPointF(cp1.x(), cp1.y()), target, perc);
            m_interpolatedPoints.replace(t, interpolatedPoint);
        }
        from_x = target_x + 1;
    }
};

// De Casteljau's algorithm for cubic bezier splines
constexpr QPointF TransferFunction::deCasteljau(QPointF p0, QPointF p1,
                                                QPointF p2, QPointF p3, float t)
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
        m_controlPoints.replace(
            index,
            clampToNeighbours(index, clampToDomain(cp, tfn::points::START_POINT,
                                                   tfn::points::END_POINT)));

        setControlNodePos(index, Nodes::NODE0,
                          old_cp.getControlNodes().value(Nodes::NODE0) +
                              (cp - old_cp));
        setControlNodePos(index, Nodes::NODE1,
                          old_cp.getControlNodes().value(Nodes::NODE1) +
                              (cp - old_cp));

        ControlPoint left_cp = m_controlPoints.at(index - 1);
        setControlNodePos(index - 1, Nodes::NODE0,
                          left_cp.getControlNodes().value(Nodes::NODE0));
        setControlNodePos(index - 1, Nodes::NODE1,
                          left_cp.getControlNodes().value(Nodes::NODE1));
    }
};

QPointF TransferFunction::clampToDomain(ControlPoint point, QPointF min,
                                        QPointF max)
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

void TransferFunction::setControlNodePos(int index, Nodes node, QPointF pos)
{
    if (index != m_controlPoints.length() - 1)
    {
        ControlPoint& cp0 = m_controlPoints[index];
        ControlPoint& cp1 = m_controlPoints[index + 1];
        const QPointF max = QPointF(cp1.x(), qMax(cp0.y(), cp1.y()));
        const QPointF min = QPointF(cp0.x(), qMin(cp0.y(), cp1.y()));
        cp0.setControlNode(node, clampToDomain(pos, min, max));
    }
};

ControlPoint::ControlPoint(QPointF position)
{
    this->setX(position.x());
    this->setY(position.y());
    m_controlNodes.insert(Nodes::NODE0, position);
    m_controlNodes.insert(Nodes::NODE1, position);
};

void ControlPoint::setControlNode(Nodes node, QPointF pos)
{
    m_controlNodes.insert(node, pos);
};

void ControlPoint::setAllControlNodes(QList<QPointF> nodes)
{
    setControlNode(Nodes::NODE0, nodes.at(0));
    setControlNode(Nodes::NODE1, nodes.at(1));
};

} // namespace tfn
