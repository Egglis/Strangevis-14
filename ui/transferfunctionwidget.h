#ifndef TRANSFERFUNCTIONWIDGET_H
#define TRANSFERFUNCTIONWIDGET_H

#include "../transfertexture.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QOpenGLTexture>
#include <QSlider>
#include <QWidget>
#include <QGraphicsView>
#include <QtCharts>

class ISharedProperties;
/*
TODO:
Add DataPoints
Add Colormaps
Add more settings

*/
namespace tfn
{

class ColorMapChart : public QChartView{
    Q_OBJECT
  public:
    ColorMapChart();
    void updateChart();
    void setDisplayedColorMap(ColorMap cmap);
   private:
    ColorMap m_currentColorMap;
    QChart *m_chart;
    QLineSeries *m_lineSeries;
    QAreaSeries *m_areaSeries;
    QScatterSeries *m_controlPointSeries;
    QPen *m_pen;
    QLinearGradient m_gradient;
};


class ColorMapSelector : public QComboBox
{
    Q_OBJECT
  public:
    ColorMapSelector(QWidget* parent, std::vector<QString> options);
};

class TransferWidget : public QWidget
{
    Q_OBJECT
  public:
    TransferWidget(const std::shared_ptr<ISharedProperties> properties, const std::shared_ptr<const IColorMapStore> colorMapStore,
                   QWidget* parent);
  public slots:
    void setSelectedColorMap(const QString& name);
  signals:
    void valueChanged(const QString& cmap);

  private:
    const std::shared_ptr<ISharedProperties> m_properties;
    const std::shared_ptr<const IColorMapStore> m_colorMapStore;
    QHBoxLayout* m_layout;
    QString m_selectedColorMap;
    ColorMapSelector* m_selector;
    ColorMapChart* m_colorMapChart;
};

} // namespace tfn
#endif // TRANSFERFUNCTIONWIDGET_H
