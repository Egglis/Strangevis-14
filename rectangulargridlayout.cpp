#include "rectangulargridlayout.h"

#include <QDebug>

RectangularGridLayout::RectangularGridLayout(QWidget* parent)
    : QVBoxLayout(parent)
{
    if(appendOnRow)
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


void RectangularGridLayout::addRow()
{
    rows.push_back(new QHBoxLayout);
    addLayout(rows.back());
}

int RectangularGridLayout::widgetCount()
{
    int count = 0;
    for (auto& row:rows){
        count += row->count();
    }
    return count;
}
