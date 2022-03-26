#ifndef TRANSFERPROPERTIES_H
#define TRANSFERPROPERTIES_H

#include "../transfertexture.h"

#include <QObject>


namespace tfn
{
class TransferProperties : public QObject
{
    Q_OBJECT
  public:
    TransferProperties();
    TransferProperties(ColorMap cmap);
    const ColorMap& colorMap() const { return m_colorMap; };
  public slots:
    void updateTransferTexture(ColorMap cmap);
  signals:
    void transferFunctionChanged(ColorMap cmap);

  private:
    ColorMap m_colorMap;
};

} // namespace tfn
#endif // TRANSFERPROPERTIES_H
