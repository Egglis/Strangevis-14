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

    for (int i = 0; i < 4; i++)
    {
        m_controlNodes.append(QPointF(0, 0));
    }
    m_controlNodesSeries->append(m_controlNodes);
    m_chart->addSeries(m_controlNodesSeries);

    connect(m_controlNodesSeries, &QScatterSeries::pressed, this, &SplineControlSeries::setClickedIndex);
}

void SplineControlSeries::setAnchor(QPointF pos, int index)
{
    m_index = index;
    m_pos = pos;

    m_controlNodes.replace(0, QPointF(pos.x() + OFFSET.x(), pos.y()));
    m_controlNodes.replace(1, QPointF(pos.x(), pos.y() + OFFSET.y()));
    m_controlNodes.replace(2, QPointF(pos.x() - OFFSET.x(), pos.y()));
    m_controlNodes.replace(3, QPointF(pos.x(), pos.y() - OFFSET.y()));
    m_controlNodesSeries->replace(m_controlNodes);
};

void SplineControlSeries::updateControlNodes(){
    m_controlNodesSeries->replace(m_controlNodes);
}

void SplineControlSeries::setVisible(bool visible)
{
    m_controlNodesSeries->setVisible(visible);
    m_top->setVisible(visible);
    m_right->setVisible(visible);
    m_bottom->setVisible(visible);
    m_left->setVisible(visible);
};

void SplineControlSeries::setClickedIndex(const QPointF& point)
{
    if(m_currentClickedIndex == -1){
        Qt::MouseButtons pressedButton = QGuiApplication::mouseButtons();
        if(pressedButton == Qt::LeftButton){
            m_currentClickedIndex = m_controlNodes.indexOf(point);
        }
    }
};

void SplineControlSeries::controlNodeReleased(QPointF point){
    if(m_currentClickedIndex != -1) {
        m_controlNodes.replace(m_currentClickedIndex, point);
        updateControlNodes();
        m_currentClickedIndex = -1;
    }
};

void SplineControlSeries::controlNodeMoved(QPointF point){
    if(m_currentClickedIndex != -1) {
        m_controlNodes.replace(m_currentClickedIndex, point);
        updateControlNodes();
    }
};


}; // namespace tfn