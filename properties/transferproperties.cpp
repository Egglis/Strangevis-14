#include "transferproperties.h"
namespace tfn
{
TransferProperties::TransferProperties(){};

TransferProperties::TransferProperties(QString cmap, TransferFunction tfn) : m_colorMap{cmap}, m_tfn{tfn} {};

void TransferProperties::updateColorMap(QString cmap)
{
    m_colorMap = cmap;
    emit colorMapChanged(cmap);
};

void TransferProperties::updateTransferFunction(TransferFunction tfn){
    m_tfn = tfn;
    emit transferFunctionChanged(tfn);
};

} // namespace tfn
