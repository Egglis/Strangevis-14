#include "environment.h"

Environment::Environment(QObject* parent) : QObject(parent)
{
    m_volume = new Volume(this);
    m_transfertexture = new TransferTexture(this);
}

Volume* Environment::volume() { return m_volume; };

TransferTexture* Environment::transferTexture() { return m_transfertexture; };