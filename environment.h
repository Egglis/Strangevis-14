#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QObject>
#include "volume.h"
#include "transfertexture.h"

class Environment : public QObject {
    Q_OBJECT
  public:
    explicit Environment(QObject* parent = nullptr);

    Volume* volume();
    TransferTexture* transferTexture();

  private:
    Volume* m_volume;
    TransferTexture* m_transfertexture;
};

#endif // ENVIRONMENT_H
