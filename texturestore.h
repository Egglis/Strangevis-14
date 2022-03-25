#ifndef TEXTURESTORE_H
#define TEXTURESTORE_H

#include "transfertexture.h"
#include "volume.h"

#include <QObject>
    Q_OBJECT
  public:
    explicit TextureStore(QObject* parent = nullptr);

    Volume* volume();
    TransferTexture* transferFunction();

  private:
    Volume* m_volume;
    TransferTexture* m_transfertexture;
};

#endif // TEXTURESTORE_H
