#ifndef SLICINGPLANEPROPERTIES_H
#define SLICINGPLANEPROPERTIES_H

#include <QObject>

enum class SlicingPlane {
  Axial,
  Coronal,
  Sagittal
};

class SlicingPlaneProperties : public QObject
{
    Q_OBJECT
    public:
    SlicingPlaneProperties();


    public slots:
    void incrementSlice();
    void decrementSlice();
    void changeSlice(int slice);
    void changePlane(SlicingPlane plane);
    void flipSliceHorizontal(bool flip);
    void flipSliceVertical(bool flip);

    signals:
    void currentSliceChanged(int slice);
    void currentPlaneChanged(SlicingPlane plane);
    void flipHorizontal(bool flip);
    void flipVertical(bool flip);

    private:
    std::array<int, 3> m_slice; // [axialSlice, coronalSlice, sagittalSlice]
    SlicingPlane m_currentPlane;
    bool m_flipHorizontal;
    bool m_flipVertical;
};


#endif // SLICINGPLANEPROPERTIES_H
