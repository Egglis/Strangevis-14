#include "slicingplaneproperties.h"

SlicingPlaneProperties::SlicingPlaneProperties() : m_slice{0,0,0}, m_currentPlane{SlicingPlane::Axial}
{
}

void SlicingPlaneProperties::changeSlice(int slice)
{
    m_slice[static_cast<int>(m_currentPlane)] = slice;
    emit currentSliceChanged(slice);
}

void SlicingPlaneProperties::changePlane(SlicingPlane plane)
{
    m_currentPlane = plane;
    emit currentPlaneChanged(plane);
    emit currentSliceChanged(m_slice[static_cast<int>(plane)]);
}

void SlicingPlaneProperties::incrementSlice()
{
    int slice = m_slice[static_cast<int>(m_currentPlane)]++;
    emit currentSliceChanged(slice);
}

void SlicingPlaneProperties::decrementSlice()
{
    int slice = m_slice[static_cast<int>(m_currentPlane)]--;
    emit currentSliceChanged(slice);
}

void SlicingPlaneProperties::flipSliceHorizontal(bool flip)
{
    m_flipHorizontal = flip;
    emit flipHorizontal(flip);
}

void SlicingPlaneProperties::flipSliceVertical(bool flip)
{
    m_flipVertical = flip;
    emit flipVertical(flip);
}
