#include "rectangulargridlayout.h"

#include <QDebug>

RectangularGridLayout::RectangularGridLayout(QWidget* parent)
    : QVBoxLayout(parent)
{
    if (appendOnRow)
        addRow();
}

void RectangularGridLayout::addWidgetRectangular(QWidget* widget)
{
    int rowCount = rows.size();
    int columnCount = rows.size() > 0 ? rows.front()->count() : 0;
    if (widgetCount() == rowCount * columnCount) // Currently rectangular
    {
        // When rectangular, switch from appending to last column or row to new
        // row or column.
        appendOnRow = !appendOnRow;
        lastAddedIndex = 0;
        if (appendOnRow)
        {
            addRow();
        }
    }

    if (appendOnRow)
    {
        rows.back()->addWidget(widget);
    }
    else
    {
        rows.at(lastAddedIndex++)->addWidget(widget);
    }
}

void RectangularGridLayout::removeWidgetRectangular(QWidget* widget)
{
    if (appendOnRow)
    {
        rows.back()->removeWidget(widget);
        if (rows.back()->count() == 0)
        {
            delete rows.back();
            rows.pop_back();
        }
    }
    else
    {
        int i = lastAddedIndex > 0 ? --lastAddedIndex : 0;
        rows.at(i)->removeWidget(widget);
        if (rows.at(i)->count() <= 0)
        {
            qDebug() << "Empty row";
        }
    }
}

void RectangularGridLayout::addRow()
{
    rows.push_back(new QHBoxLayout);
    addLayout(rows.back());
}

int RectangularGridLayout::widgetCount()
{
    int count = 0;
    for (auto& row : rows)
    {
        count += row->count();
    }
    return count;
}
