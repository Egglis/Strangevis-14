#include "volume.h"

#include "vendor/inireader/INIReader.h"

#include <QDataStream>
#include <QDebug>
#include <QFile>


Volume::Volume(QObject* parent)
    : QObject(parent), m_dims{0, 0, 0},
      m_volumeTexture(QOpenGLTexture::Target3D), m_updateNeeded(false)
{
}

void Volume::load(const QString& fileName)
{
    VolumeLoader* volumeLoader = new VolumeLoader(fileName, this);
    connect(volumeLoader, &VolumeLoader::volumeLoaded,
            [this](const std::vector<unsigned short>& volumeData) {
                m_updateNeeded = true;
                m_volumeData = volumeData;
                emit volumeLoaded();
            });
    connect(volumeLoader, &VolumeLoader::dimensionsChanged,
            [this](QVector3D dims) {
                m_dims = dims;
                emit dimensionsChanged(m_dims);
            });
    connect(volumeLoader, &VolumeLoader::loadingStartedOrStopped, this,
            &Volume::loadingStartedOrStopped);
    connect(volumeLoader, &VolumeLoader::finished, volumeLoader,
            &VolumeLoader::deleteLater);
    connect(volumeLoader, &VolumeLoader::gridSpacingChanged, this, &Volume::gridSpacingChanged);
    volumeLoader->start();
}

VolumeLoader::VolumeLoader(const QString& fileName, QObject* parent)
    : QThread{parent}, m_fileName{fileName}
{
}
void VolumeLoader::run()
{
    loadIni();
    load();
}

void VolumeLoader::loadIni()
{
    QString fileName = m_fileName;
    fileName.chop(3);
    fileName.append("ini");

    INIReader reader{fileName.toStdString()};
    if (reader.ParseError() != 0)
    {
        qDebug() << "INI-file not loaded.";
        emit gridSpacingChanged(UNIFORM_GRID_DIMENSIONS);
    }
    float x = reader.GetFloat("DatFile", "oldDat Spacing X", 1);
    float y = reader.GetFloat("DatFile", "oldDat Spacing Y", 1);
    float z = reader.GetFloat("DatFile", "oldDat Spacing Z", 1);
    qDebug() << "X:" << x << "Y:" << y << "Z:" << z;
    emit gridSpacingChanged(QVector3D(x,y,z));

}

void VolumeLoader::load()
{
    QFile file(m_fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Unable to open " << m_fileName << "!";
        return;
    }
    emit loadingStartedOrStopped(true);
    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);

    unsigned short width = 0, height = 0, depth = 0;
    stream >> width >> height >> depth;

    qDebug() << "Width:" << width;
    qDebug() << "Height:" << height;
    qDebug() << "Depth:" << depth;

    std::vector<unsigned short> volumeData;
    int volumeSize = static_cast<int>(width) * static_cast<int>(height) *
                     static_cast<int>(depth);
    volumeData.resize(volumeSize);
    if (stream.readRawData(reinterpret_cast<char*>(volumeData.data()),
                           volumeSize * sizeof(unsigned short)) !=
        volumeSize * sizeof(unsigned short))
    {
        emit loadingStartedOrStopped(false);
        return;
    }
    for (int i = 0; i < volumeSize; i++)
    {
        volumeData[i] *= 16;
    }
    emit volumeLoaded(volumeData);
    emit dimensionsChanged(QVector3D(width, height, depth));
    emit loadingStartedOrStopped(false);
}

void Volume::bind()
{
    if (m_updateNeeded)
    {
        initializeOpenGLFunctions();

        if (m_volumeTexture.isCreated())
        {
            m_volumeTexture.destroy();
        }
        m_volumeTexture.setBorderColor(0, 0, 0, 0);
        m_volumeTexture.setWrapMode(QOpenGLTexture::ClampToBorder);
        m_volumeTexture.setFormat(QOpenGLTexture::R16F);
        m_volumeTexture.setMinificationFilter(QOpenGLTexture::Linear);
        m_volumeTexture.setMagnificationFilter(QOpenGLTexture::Linear);
        m_volumeTexture.setAutoMipMapGenerationEnabled(false);
        m_volumeTexture.setSize(m_dims.x(), m_dims.y(), m_dims.z());
        m_volumeTexture.allocateStorage();

        void* data = reinterpret_cast<void*>(m_volumeData.data());
        m_volumeTexture.setData(0, 0, 0, m_dims.x(), m_dims.y(), m_dims.z(),
                                QOpenGLTexture::Red, QOpenGLTexture::UInt16,
                                data);
        m_updateNeeded = false;
    }

    if (m_volumeTexture.isCreated())
    {
        m_volumeTexture.bind();
    }
}

void Volume::release()
{
    if (m_volumeTexture.isCreated())
    {
        m_volumeTexture.release();
    }
}
