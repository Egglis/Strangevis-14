#include "hintitem.h"

namespace tfn
{

HintItem::HintItem(QChart* chart, QScatterSeries* series)
    : QGraphicsItem(chart), m_chart(chart), m_series(series){};

QRectF HintItem::boundingRect() const
{
    QPointF anchor = mapFromParent(m_chart->mapToPosition(m_anchor, m_series));
    QRectF rect;
    rect.setLeft(qMin(m_textRect.left(), anchor.x()));
    rect.setRight(qMax(m_textRect.right(), anchor.x()));
    rect.setTop(qMin(m_textRect.top(), anchor.y()));
    rect.setBottom(qMax(m_textRect.bottom(), anchor.y()));
    return rect;
};

void HintItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                     QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QPointF anchor = mapFromParent(m_chart->mapToPosition(m_anchor, m_series));
    painter->setBrush(FILL_COLOR);
    painter->drawRect(m_rect);
    painter->drawText(m_textRect, m_text);
    painter->restore();
};

void HintItem::setText(const QString& text)
{
    m_text = text;
    QFontMetrics metrics(m_font);
    m_textRect =
        metrics.boundingRect(QRect(0, 0, 150, 150), Qt::AlignLeft, m_text);
    m_textRect.translate(5, 5);
    m_rect = m_textRect.adjusted(-2, -2, 2, 2);
    prepareGeometryChange();
};

void HintItem::setAnchor(QPointF point, QPointF offset)
{
    m_anchor = point;
    setPos(m_chart->mapToPosition(point) + offset);
};

}; // namespace tfn