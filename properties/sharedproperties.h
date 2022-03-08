#ifndef SHAREDPROPERTIES_H
#define SHAREDPROPERTIES_H

#include <QVector3D>
#include <QObject>

class SharedProperties : public QObject
{
    Q_OBJECT
  public:
    SharedProperties();

    const QVector4D& clippingPlane() {return m_clippingPlane;};
    void updateClippingPlane(QVector4D clippingPlane);

  signals:
    void clippingPlaneChanged(const QVector4D& newClippingPlane);

  private:
    QVector4D m_clippingPlane;
};

#endif // SHAREDPROPERTIES_H
