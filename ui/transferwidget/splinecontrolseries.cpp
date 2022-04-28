#include "splinecontrolseries.h"

namespace tfn
{

SplineControlSeries::SplineControlSeries(TransferFunction* tfn) : m_tfn{tfn}
{
    m_controlNodesSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    m_controlNodesSeries->setMarkerSize(6);
    m_controlNodesSeries->setBorderColor(Qt::green);
    m_controlNodesSeries->setColor(Qt::gray);

    for (int i = 0; i < 2; i++)
    {
        m_controlNodes.append(QPointF(0, 0));
    }
    m_n0LineSeries->append(QPointF(0, 0));
    m_n0LineSeries->append(QPointF(0, 0));
    m_n1LineSeries->append(QPointF(0, 0));
    m_n1LineSeries->append(QPointF(0, 0));

    m_controlNodesSeries->append(m_controlNodes);
    setVisible(false);

    connect(m_controlNodesSeries, &QScatterSeries::pressed, this,
            &SplineControlSeries::setClickedNode);
}

void SplineControlSeries::setAnchor(int index)
{
    m_index = index;
    updateControlNodes();
};

void SplineControlSeries::setVisible(bool visible)
{
    m_controlNodesSeries->setVisible(visible);
    m_n0LineSeries->setVisible(visible);
    m_n1LineSeries->setVisible(visible);
};

bool SplineControlSeries::isVisible()
{
    return m_controlNodesSeries->isVisible();
}

void SplineControlSeries::updateControlNodes()
{
    ControlPoint cp =
        static_cast<ControlPoint>(m_tfn->getControlPoints().at(m_index));
    m_controlNodesSeries->replace(0,
                                  cp.getControlNodes().value(Nodes::NODE0));
    m_controlNodesSeries->replace(1,
                                  cp.getControlNodes().value(Nodes::NODE1));
    m_n0LineSeries->replace(0, cp);
    m_n1LineSeries->replace(0, cp);
    m_n0LineSeries->replace(1, cp.getControlNodes().value(Nodes::NODE0));
    m_n1LineSeries->replace(1, cp.getControlNodes().value(Nodes::NODE1));
}

void SplineControlSeries::setClickedNode(const QPointF& point)
{

    if (m_currentClickedNode == Nodes::INVALID_NODE)
    {
        const Qt::MouseButtons pressedButton = QGuiApplication::mouseButtons();
        const Qt::KeyboardModifiers pressedKey =
            QGuiApplication::keyboardModifiers();

        if (pressedButton == Qt::LeftButton)
        {
            m_isLinked = (pressedKey == Qt::ShiftModifier);
            m_currentClickedNode =
                static_cast<ControlPoint>(m_tfn->getControlPoints().at(m_index))
                    .getControlNodes()
                    .key(point);

        }
    }
};

bool SplineControlSeries::controlNodeReleased(QPointF point)
{
    if (m_currentClickedNode != Nodes::INVALID_NODE && isVisible())
    {
        if (m_isLinked)
        {
            m_tfn->setControlNodePos(m_index, m_currentClickedNode == Nodes::NODE0 ? Nodes::NODE1 : Nodes::NODE0,
                                     point);
        }
        m_tfn->setControlNodePos(m_index, m_currentClickedNode, point);
        m_currentClickedNode = Nodes::INVALID_NODE;
        updateControlNodes();
        return true;
    }
    return false;
};

bool SplineControlSeries::controlNodeMoved(QPointF point)
{
    if (m_currentClickedNode != Nodes::INVALID_NODE && isVisible())
    {
        if (m_isLinked)
        {
            m_tfn->setControlNodePos(m_index, m_currentClickedNode == Nodes::NODE0 ? Nodes::NODE1 : Nodes::NODE0,
                                     point);
        }
        m_tfn->setControlNodePos(m_index, m_currentClickedNode, point);
        updateControlNodes();
        return true;
    }
    return false;
};

}; // namespace tfn