#include "transfertexture.h"

TransferTexture::TransferTexture(QObject* parent)
    : QObject(parent), m_transferTexture(QOpenGLTexture::Target1D),
      m_updateNeeded(false)
{
    ColorMap cmap = ColorMap();
    m_colorMap = cmap;
    m_updateNeeded = true;
};

void TransferTexture::setColorMap(ColorMap colormap)
{
    m_colorMap = colormap;
    m_updateNeeded = true;
};

/*
void TransferTexture::updateA(float value){
    m_colorMap[1] = value;
    m_updateNeeded = true;
}
*/

void TransferTexture::bind()
{
    if (m_updateNeeded)
    {
        qDebug() << "Colormap Texture Created";

        if (m_transferTexture.isCreated())
        {
            m_transferTexture.destroy();
        }

        m_transferTexture.setBorderColor(0, 0, 0, 0);
        m_transferTexture.setWrapMode(QOpenGLTexture::ClampToEdge);
        m_transferTexture.setFormat(QOpenGLTexture::RGBA32F);
        m_transferTexture.setMinificationFilter(QOpenGLTexture::Nearest);
        m_transferTexture.setMagnificationFilter(QOpenGLTexture::Nearest);
        m_transferTexture.setAutoMipMapGenerationEnabled(false);
        m_transferTexture.setMipLevels(0);
        m_transferTexture.setSize(256);
        m_transferTexture.allocateStorage();
        m_transferTexture.setData(QOpenGLTexture::RGBA, QOpenGLTexture::Float32,
                                  m_colorMap.m_colorMapData.data());

        m_updateNeeded = false;

        if (m_transferTexture.isCreated())
        {
            m_transferTexture.bind(1);
        }
    }
};

void TransferTexture::release()
{
    if (m_transferTexture.isCreated())
    {
        m_transferTexture.release();
    }
};

ColorMap::ColorMap(QString name, std::vector<GLfloat>& data)
    : m_name{name}, m_colorMapData{data}
{
    qDebug() << "new Colormap" << name;
};

ColorMap::ColorMap()
{
    // Defualt RGB ColorMap, i know there is a better way, but i am lazy
    m_name = "RGB";
    for (int i = 0; i < 256 * 4; i++)
    {
        m_colorMapData.push_back(0);
    }
}
