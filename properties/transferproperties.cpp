#include "transferproperties.h"
namespace tfn
{
TransferProperties::TransferProperties(){};

TransferProperties::TransferProperties(ColorMap cmap) : m_colorMap{cmap} {};

void TransferProperties::updateTransferTexture(ColorMap cmap)
{
    m_colorMap = cmap;
    emit transferFunctionChanged(cmap);
}
} // namespace tfn
