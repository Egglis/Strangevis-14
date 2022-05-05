#ifndef TRANSFERFUNCTIONWIDGET_H
#define TRANSFERFUNCTIONWIDGET_H

#include "../transfertexture.h"
#include "transferfunctiongraph.h"

#include <QOpenGLTexture>
#include <QWidget>

class ISharedProperties;
class QHBoxLayout;

namespace tfn
{

class ColorMapSelector : public QListWidget
{
    Q_OBJECT
  public:
    ColorMapSelector(
        QWidget* parent,
        const std::shared_ptr<const IColorMapStore> m_colorMapStore);
};

class TransferWidget : public QWidget
{
    Q_OBJECT
  public:
    TransferWidget(const std::shared_ptr<ISharedProperties> properties,
                   const std::shared_ptr<const IColorMapStore> colorMapStore,
                   QWidget* parent);
  TransferFunctionGraph* getGraph() {return m_tfnGraph;};
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
    TransferFunctionGraph* m_tfnGraph;
};

} // namespace tfn
#endif // TRANSFERFUNCTIONWIDGET_H
