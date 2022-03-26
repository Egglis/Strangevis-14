#ifndef VOLUME_H
#define VOLUME_H

#include <QObject>
#include <QOpenGLExtraFunctions>
#include <QOpenGLTexture>
#include <QThread>
#include <QVector3D>
#include <QVector>

class Volume : public QObject, protected QOpenGLExtraFunctions
{
    Q_OBJECT
  public:
    explicit Volume(QObject* parent = nullptr);
    void load(const QString& filename);
    const QVector3D& getDimensions() const { return m_dims; };

    void bind();
    void release();
  signals:
    void volumeLoaded();
    void dimensionsChanged(QVector3D dims);
    void loadingStartedOrStopped(bool started);

  private:
    QVector<unsigned short> m_volumeData;
    QVector3D m_dims;
    QOpenGLTexture m_volumeTexture;
    bool m_updateNeeded;
};

class VolumeLoader : public QThread
{
    Q_OBJECT
  public:
    VolumeLoader(const QString& fileName, QObject* parent);
    void run() override;

  signals:
    void volumeLoaded(QVector<unsigned short> m_volumeData);
    void dimensionsChanged(QVector3D dims);
    void loadingStartedOrStopped(bool started);

  private:
    void load();
    QString m_fileName;
};

#endif // VOLUME_H
