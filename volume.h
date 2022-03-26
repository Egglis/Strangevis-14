#ifndef VOLUME_H
#define VOLUME_H

#include <QObject>
#include <QOpenGLExtraFunctions>
#include <QOpenGLTexture>
#include <QVector>
#include <QVector3D>

class Volume : public QObject, protected QOpenGLExtraFunctions
{
    Q_OBJECT
  public:
    explicit Volume(QObject* parent = nullptr);
    bool load(const QString& filename);
    QVector3D getDimensions() const { return QVector3D(m_width, m_height, m_depth); };

    void bind();
    void release();
    signals:
    void dimensionsChanged(QVector3D dims);

  private:
    QVector<unsigned short> m_volumeData;
    int m_width, m_height, m_depth;
    QOpenGLTexture m_volumeTexture;
    bool m_updateNeeded;
};

#endif // VOLUME_H
