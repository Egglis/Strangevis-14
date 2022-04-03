#ifndef TRANSFERPROPERTIES_H
#define TRANSFERPROPERTIES_H

#include "../transfertexture.h"
#include "../transferfunction.h"

#include <QObject>


namespace tfn
{
class TransferProperties : public QObject
{
    Q_OBJECT
  public:
    TransferProperties();
    TransferProperties(QString cmap, TransferFunction tfn);
    const QString& colorMap() const { return m_colorMap; };
    const TransferFunction& tfn() const {return m_tfn; };
  public slots:
    void updateColorMap(QString cmap);
    void updateTransferFunction(TransferFunction tfn);
  signals:
    void colorMapChanged(const QString& cmap);
    void transferFunctionChanged(const TransferFunction& tfn);

  private:
    QString m_colorMap;
    TransferFunction m_tfn;
};

} // namespace tfn
#endif // TRANSFERPROPERTIES_H
