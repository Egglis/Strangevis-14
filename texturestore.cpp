#include "texturestore.h"

TextureStore::TextureStore(QObject* parent)
    : QObject(parent), m_volume{this}, m_transfertexture{this}
{
}
