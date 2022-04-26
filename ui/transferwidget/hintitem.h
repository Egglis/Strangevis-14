#ifndef HINTITEM_H
#define HINTITEM_H

#include <QColor>
#include <QtCharts>
#include <QtWidgets/QGraphicsItem>

namespace tfn
{

class HintItem : public QGraphicsItem
{
  public:
    HintItem(QChart* parent, QScatterSeries* series);
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;

    void setText(const QString& text);
    void setAnchor(QPointF point, QPointF offset);

  private:
    QColor FILL_COLOR = QColor(178, 178, 178, 170);
    QColor TEXT_COLOR = QColor(0, 0, 0);
    int MARGIN = 2;

    QChart* m_chart;
    QScatterSeries* m_series;
    QString m_text;
    QRectF m_rect;
    QFont m_font;
    QRectF m_textRect;
    QPointF m_anchor;
};

}; // namespace tfn

#endif