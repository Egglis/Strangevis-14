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
    TransferProperties(QString cmap);
    const QString& colorMap() const { return m_colorMap; };
  public slots:
    void updateTexture(QString cmap);
  signals:
    void transferFunctionChanged(const QString& cmap);

  private:
    QString m_colorMap;
};

} // namespace tfn
#endif // TRANSFERPROPERTIES_H
