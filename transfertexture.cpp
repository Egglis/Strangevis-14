#include "transfertexture.h"

#include <QFile>
#include <QtXml>
#include <QVector4D>

namespace tfn
{

TransferTexture::TransferTexture(QObject* parent)
    : QObject(parent), m_transferTexture(QOpenGLTexture::Target1D),
      m_updateNeeded{true}, m_colorMap{}
{
}

void TransferTexture::setColorMap(ColorMap colormap)
{
    m_colorMap = colormap;
    m_updateNeeded = true;
};

void TransferTexture::bind()
{
    if (m_updateNeeded)
    {
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
        m_transferTexture.setSize(tfn::size::NumPoints);
        m_transferTexture.allocateStorage();
        m_transferTexture.setData(QOpenGLTexture::RGB, QOpenGLTexture::Float32,
                                  m_colorMap.colorMapData().data());

        m_updateNeeded = false;
    }
    if (m_transferTexture.isCreated())
    {
        m_transferTexture.bind();
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
    : m_name{name}, m_colorMapData{data} {};

ColorMap::ColorMap() : m_name{"Ramp Grey"}
{
    using namespace size;
    m_colorMapData.reserve(ArraySize);
    for (int i = 0; i < ArraySize; i += NumChannels)
    {
        for (int j = 0; j < NumChannels; j++)
        {
            m_colorMapData.push_back((i / NumChannels) /
                                     static_cast<GLfloat>(NumPoints - 1));
        }
    }
    assert(m_colorMapData.size() == ArraySize);
}

std::vector<ColorMap> loadColorMapsFromFile()
{
    // Open file for Reading
    std::vector<ColorMap> colorMaps{};
    QDomDocument xml;
    QFile f(":cmaps/cmaps.xml");
    if (!f.open(QIODevice::ReadOnly))
    {
        // Error:
        qDebug() << "Error while loading file";
        return std::vector<ColorMap>{};
    }
    xml.setContent(&f);
    f.close();

    // Extract data:
    QDomElement root = xml.documentElement();
    QDomElement component = root.firstChild().toElement();
    while (!component.isNull())
    {
        if (component.tagName() == "ColorMap")
        {
            QString name = component.attribute("name");
            QString space = component.attribute("space");
            if (space != "RGB")
            {
                component = component.nextSibling().toElement();
                continue;
            }

            // Extract RGB values and alpha values
            QDomElement child = component.firstChild().toElement();
            std::vector<GLfloat> colors{};
            colors.reserve(size::ArraySize);
            std::vector<QVector4D> sortableColors{};
            sortableColors.reserve(size::NumPoints);

            int index = 0;
            while (!child.isNull())
            {
                if (child.tagName() == "Point")
                {
                    auto r = child.attribute("r").toFloat();
                    auto g = child.attribute("g").toFloat();
                    auto b = child.attribute("b").toFloat();
                    auto x = child.attribute("x").toFloat(); // Sorting parameter
                    sortableColors.push_back(QVector4D(r,g,b,x));
                }
                child = child.nextSibling().toElement();
            }
            std::sort(sortableColors.begin(), sortableColors.end(), [](const auto& c1, const auto& c2){
                return c1.w() < c2.w();
            });
            for (const auto& color : sortableColors)
            {
                colors.push_back(color.x()); // r
                colors.push_back(color.y()); // g
                colors.push_back(color.z()); // b
            }
            assert(colors.size() == size::ArraySize);
            ColorMap cmap = ColorMap(name, colors);
            colorMaps.push_back(cmap);
        }
        component = component.nextSibling().toElement();
    }

    return colorMaps;
};
} // namespace tfn
