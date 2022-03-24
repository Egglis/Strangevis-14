#include "transferproperties.h"

TransferProperties::TransferProperties()
{};

TransferProperties::TransferProperties(ColorMap cmap) : m_colorMap{cmap}
{};

void TransferProperties::updateTransferTexture(ColorMap cmap){
    m_colorMap = cmap;
    emit transferTextureChanged(cmap);
}