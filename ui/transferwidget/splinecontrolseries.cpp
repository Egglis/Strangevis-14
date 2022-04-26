#include "splinecontrolseries.h"

namespace tfn
{


SplineControlSeries::SplineControlSeries(TransferFunction* tfn, QChart* chart)
    : m_tfn{tfn}, m_chart{chart}
{
    m_controlNodesSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    m_controlNodesSeries->setMarkerSize(6);
    m_controlNodesSeries->setBorderColor(Qt::green);
    m_controlNodesSeries->setColor(Qt::gray);

    for (int i = 0; i < 2; i++)
    {
        m_controlNodes.append(QPointF(0, 0));
    }
    m_controlNodesSeries->append(m_controlNodes);
    m_chart->addSeries(m_controlNodesSeries);

    connect(m_controlNodesSeries, &QScatterSeries::pressed, this,
            &SplineControlSeries::setClickedIndex);
}

void SplineControlSeries::setAnchor(int index)
{
    m_index = index;
    m_controlNodesSeries->replace(
        static_cast<ControlPoint>(m_tfn->getControlPoints().at(index))
            .getControlNodes());
};

void SplineControlSeries::updateControlNodes()
{
    m_controlNodesSeries->replace(
        static_cast<ControlPoint>(m_tfn->getControlPoints().at(m_index))
            .getControlNodes());
}

void SplineControlSeries::setVisible(bool visible)
{
    m_controlNodesSeries->setVisible(visible);
    m_top->setVisible(visible);
    m_right->setVisible(visible);
};

void SplineControlSeries::setClickedIndex(const QPointF& point)
{
    if (m_currentClickedNode == -1)
    {
        Qt::MouseButtons pressedButton = QGuiApplication::mouseButtons();
        if (pressedButton == Qt::LeftButton)
        {
            m_currentClickedNode =
                static_cast<ControlPoint>(m_tfn->getControlPoints().at(m_index))
                    .getControlNodes()
                    .indexOf(point);
        }
    }
};

bool SplineControlSeries::controlNodeReleased(QPointF point)
{
    if (m_currentClickedNode != -1)
    {
        m_tfn->setControlNodePos(m_index, m_currentClickedNode, point);
        m_currentClickedNode = -1;
        updateControlNodes();
        return true;
    }
    return false;
};

bool SplineControlSeries::controlNodeMoved(QPointF point)
{
    if (m_currentClickedNode != -1)
    {
        m_tfn->setControlNodePos(m_index, m_currentClickedNode, point);
        updateControlNodes();
        return true;
    }
    return false;
};

}; // namespace tfn