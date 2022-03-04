#ifndef RECTANGULARGRIDLAYOUT_H
#define RECTANGULARGRIDLAYOUT_H
#include <QVBoxLayout>

class RectangularGridLayout : public QVBoxLayout
{
    Q_OBJECT
  public:
    RectangularGridLayout(QWidget* parent);
    void addWidgetRectangular(QWidget* widget);
    void removeWidgetRectangular(QWidget* widget);

  private:
    void addRow();
    int widgetCount();
    bool appendOnRow = false;
    int lastAddedIndex = 0;
    QVector<QHBoxLayout*> rows;
};
#endif // RECTANGULARGRIDLAYOUT_H
