#ifndef VOLUME_H
#define VOLUME_H

#include <QObject>
#include <QOpenGLExtraFunctions>
#include <QOpenGLTexture>
#include <QVector>

class Volume : public QObject, protected QOpenGLExtraFunctions
{
    Q_OBJECT
  public:
    explicit Volume(QObject* parent = nullptr);
    bool load(const QString& filename);
    std::tuple<int,int,int> getDimensions() { return std::tuple<int, int, int>{m_width, m_height, m_depth}; };

    void bind();
    void release();

  private:
    QVector<unsigned short> m_volumeData;
    int m_width, m_height, m_depth;
    QOpenGLTexture m_volumeTexture;
    bool m_updateNeeded;
};

#endif // VOLUME_H
