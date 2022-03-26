#include "transferproperties.h"
namespace tfn
{
TransferProperties::TransferProperties(){};

TransferProperties::TransferProperties(QString cmap) : m_colorMap{cmap} {};

void TransferProperties::updateTexture(QString cmap)
{
    m_colorMap = cmap;
    emit transferFunctionChanged(cmap);
}
} // namespace tfn
