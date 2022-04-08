#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H

#include <QtCharts>

class HistogramWidget : public QChartView
{
    Q_OBJECT
    public:
    HistogramWidget(QWidget* parent = nullptr);
    public slots:
    void histogramChanged(std::vector<float> normalizedHistogramData);

    private:
    QList<QPointF> dataAsList(const std::vector<float>& normalizedHistogramData);

    QChart* m_chart;
    QLineSeries* m_series;
};

#endif // HISTOGRAMWIDGET_H
