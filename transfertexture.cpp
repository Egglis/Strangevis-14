#include "transfertexture.h"

#include <QFile>
#include <QVector4D>
#include <QtXml>

namespace tfn
{

TransferTexture::TransferTexture(QObject* parent)
    : QObject(parent), m_transferTexture(QOpenGLTexture::Target1D),
      m_updateNeeded{true}, m_colorMap{}
{
}

void TransferTexture::setColorMap(std::vector<GLfloat> colormap)
{
    m_colorMap = colormap;
    m_updateNeeded = true;
};

void TransferTexture::setTransferFunction(TransferFunction tfn)
{
    m_tfn = tfn;
    m_updateNeeded = true;
}

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
        m_transferTexture.setSize(tfn::size::NUM_POINTS);
        m_transferTexture.allocateStorage();
        m_transferTexture.setData(QOpenGLTexture::RGBA, QOpenGLTexture::Float32,
                                  m_tfn.getColorMapData()->data());

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
    using namespace tfn::size;
    m_colorMapData.reserve(ARRAY_SIZE);
    for (int i = 0; i < ARRAY_SIZE; i += NUM_CHANNELS)
    {
        for (int j = 0; j < NUM_CHANNELS; j++)
        {
            m_colorMapData.push_back((i / NUM_CHANNELS) /
                                     static_cast<GLfloat>(NUM_POINTS - 1));
        }
    }
    assert(m_colorMapData.size() == ARRAY_SIZE);
}

ColorMapStore::ColorMapStore() : m_defaultColorMap{ColorMap{}}
{
    loadColorMapsFromFile(":cmaps/cmaps.xml");
    addColorMap(m_defaultColorMap);
}

void ColorMapStore::addColorMap(const ColorMap& colorMap)
{
    m_colorMaps.insert(colorMap.getName(), colorMap);
}

bool ColorMapStore::loadColorMapsFromFile(QString path)
{
    // Open file for Reading
    QDomDocument xml;
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
    {
        // Error:
        qDebug() << "Error while loading file";
        return false;
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
            colors.reserve(tfn::size::ARRAY_SIZE);
            std::vector<QVector4D> sortableColors{};
            sortableColors.reserve(tfn::size::NUM_POINTS);

            while (!child.isNull())
            {
                if (child.tagName() == "Point")
                {
                    auto r = child.attribute("r").toFloat();
                    auto g = child.attribute("g").toFloat();
                    auto b = child.attribute("b").toFloat();
                    auto x =
                        child.attribute("x").toFloat(); // Sorting parameter
                    sortableColors.push_back(QVector4D(r, g, b, x));
                }
                child = child.nextSibling().toElement();
            }
            std::sort(
                sortableColors.begin(), sortableColors.end(),
                [](const auto& c1, const auto& c2) { return c1.w() < c2.w(); });
            for (const auto& color : sortableColors)
            {
                colors.push_back(color.x()); // r
                colors.push_back(color.y()); // g
                colors.push_back(color.z()); // b
                colors.push_back(color.w()); // a
            }
            assert(colors.size() == tfn::size::ARRAY_SIZE);
            ColorMap cmap = ColorMap(name, colors);
            m_colorMaps.insert(name, cmap);
        }
        component = component.nextSibling().toElement();
    }
    return true;
};

ColorMap ColorMapStore::colorMap(const QString& name) const
{
    return m_colorMaps.value(name, m_defaultColorMap);
}

std::vector<QString> ColorMapStore::availableColorMaps() const
{
    auto keys = m_colorMaps.keys();
    std::vector<QString> availableKeys{keys.begin(), keys.end()};
    return availableKeys;
}
} // namespace tfn
