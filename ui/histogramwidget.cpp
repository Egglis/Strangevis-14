#include "histogramwidget.h"

#include <execution>
HistogramWidget::HistogramWidget(QWidget* parent) : QChartView(parent)
{
    auto* layout = new QVBoxLayout();
    m_chart = new QChart();
    m_series = new QLineSeries();

    QValueAxis* axisX = new QValueAxis();
    axisX->setRange(0, 4096);
    axisX->setLabelsVisible(false);
    m_chart->addAxis(axisX, Qt::AlignBottom);

    QValueAxis* axisY = new QValueAxis();
    axisY->setRange(0, 1);
    axisY->setLabelsVisible(false);
    m_chart->addAxis(axisY, Qt::AlignLeft);

    m_chart->legend()->hide();
    m_chart->addSeries(m_series);
    setChart(m_chart);
    setLayout(layout);
    auto* checkbox = new QCheckBox("Reduce");
    layout->addStretch(1);
    layout->addWidget(checkbox);
    connect(checkbox, &QCheckBox::toggled, [this](bool checked){
        filtered = checked;
        histogramChanged(m_fullHistogram);
    });
}

void HistogramWidget::histogramChanged(
    std::vector<float> normalizedHistogramData)
{
    if ( normalizedHistogramData.size() == 0)
    {
        return;
    }
    m_fullHistogram = normalizedHistogramData;
    if (filtered)
    {
        for (int i = 0; i < 64; i++)
        {
            normalizedHistogramData[i] = 0;
        }
    }
    normalizeHistogram(normalizedHistogramData);
    m_chart->removeSeries(m_series);
    m_series->clear();

    m_series->append(dataAsList(normalizedHistogramData));
    m_chart->addSeries(m_series);
    m_series->attachAxis(m_chart->axes(Qt::Horizontal)[0]);
    m_series->attachAxis(m_chart->axes(Qt::Vertical)[0]);

    update();
}

QList<QPointF>
HistogramWidget::dataAsList(const std::vector<float>& normalizedHistogramData)
{
    QList<QPointF> list;
    for (int i = 0; i < normalizedHistogramData.size(); i++)
    {
        list.push_back(QPointF(i, normalizedHistogramData[i]));
    }
    return list;
}

void HistogramWidget::normalizeHistogram(std::vector<float>& histogramData)
{
    float maxCount =
        *std::max_element(histogramData.begin(), histogramData.end());
    std::transform(std::execution::par_unseq, histogramData.begin(),
                   histogramData.end(), histogramData.begin(),
                   [maxCount](const auto& elem) {
                       return elem / maxCount;
                   });
}
