#include "histogramwidget.h"

HistogramWidget::HistogramWidget(QWidget* parent) : QChartView(parent)
{
    m_chart = new QChart();
    m_series = new QLineSeries();

    QValueAxis* axisX = new QValueAxis();
    axisX->setRange(0, 4096);
    axisX->setLabelsVisible(false);
    m_chart->addAxis(axisX, Qt::AlignBottom);

    QValueAxis* axisY = new QValueAxis();
    axisY->setRange(0,1);
    axisY->setLabelsVisible(false);
    m_chart->addAxis(axisY, Qt::AlignLeft);

    m_chart->legend()->hide();
    m_chart->addSeries(m_series);
    setChart(m_chart);
}

void HistogramWidget::histogramChanged(std::vector<float> normalizedHistogramData)
{
    m_chart->removeSeries(m_series);
    m_series->clear();

    m_series->append(dataAsList(normalizedHistogramData));
    m_chart->addSeries(m_series);
    m_series->attachAxis(m_chart->axes(Qt::Horizontal)[0]);
    m_series->attachAxis(m_chart->axes(Qt::Vertical)[0]);

    update();
}

QList<QPointF> HistogramWidget::dataAsList(const std::vector<float>& normalizedHistogramData)
{
    QList<QPointF> list;
    for(int i = 0; i<normalizedHistogramData.size(); i++)
    {
        list.push_back(QPointF(i, normalizedHistogramData[i]));
    }
    return list;
}
