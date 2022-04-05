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
    QMatrix4x4 modelMatrix() const;

    void bind();
    void release();
  signals:
    void volumeLoaded();
    void loadingStartedOrStopped(bool started);
    void histogramCalculated(std::vector<float> histogramData);

  private:
    QVector3D scaleFactor() const;
    std::vector<unsigned short> m_volumeData;
    QVector3D m_dims;
    QVector3D m_spacing;
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
    void volumeLoaded(const std::vector<unsigned short>& m_volumeData);
    void dimensionsChanged(QVector3D dims);
    void loadingStartedOrStopped(bool started);
    void gridSpacingChanged(QVector3D dims);
    void histogramCalculated(std::vector<float> histogramData);

  private:
    void loadIni();
    void load();
    QString m_fileName;
    constexpr static QVector3D UNIFORM_GRID_DIMENSIONS{1, 1, 1};
};

#endif // VOLUME_H
