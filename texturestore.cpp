#include "texturestore.h"

TextureStore::TextureStore(QObject* parent) : QObject(parent)
{
    m_volume = new Volume(this);
    m_transfertexture = new TransferTexture(this);
}

Volume* TextureStore::volume() { return m_volume; };

TransferTexture* TextureStore::transferFunction() { return m_transfertexture; };
