#include "volume.h"

#include <QDataStream>
#include <QDebug>
#include <QFile>

Volume::Volume(QObject* parent)
    : QObject(parent), m_width(0), m_height(0), m_depth(0),
      m_volumeTexture(QOpenGLTexture::Target3D), m_updateNeeded(false)
{
}

bool Volume::load(const QString& fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Unable to open " << fileName << "!";
        return false;
    }

    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);

    unsigned short width = 0, height = 0, depth = 0;
    stream >> width >> height >> depth;

    qDebug() << "Width:" << width;
    qDebug() << "Height:" << height;
    qDebug() << "Depth:" << depth;

    int volumeSize = static_cast<int>(width) * static_cast<int>(height) *
                     static_cast<int>(depth);
    m_volumeData.resize(volumeSize);

    if (stream.readRawData(reinterpret_cast<char*>(m_volumeData.data()),
                           volumeSize * sizeof(unsigned short)) !=
        volumeSize * sizeof(unsigned short))
    {
        return false;
    }

    m_width = static_cast<int>(width);
    m_height = static_cast<int>(height);
    m_depth = static_cast<int>(depth);

    for (int i = 0; i < volumeSize; i++)
    {
        m_volumeData[i] *= 16;
    }

    m_updateNeeded = true;
    return true;
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
        m_volumeTexture.setSize(m_width, m_height, m_depth);
        m_volumeTexture.allocateStorage();

        void* data = reinterpret_cast<void*>(m_volumeData.data());
        m_volumeTexture.setData(0, 0, 0, m_width, m_height, m_depth,
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
