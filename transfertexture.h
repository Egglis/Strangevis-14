#ifndef TRANSFERTEXTURE_H
#define TRANSFERTEXTURE_H

#include "transferfunction.h"

#include <QColor>
#include <QMap>
#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <vector>

namespace tfn
{

namespace size
{
constexpr static int NUM_POINTS = 256;
constexpr static int NUM_CHANNELS = 4;
constexpr static int ARRAY_SIZE = NUM_POINTS * NUM_CHANNELS;
} // namespace size
class ColorMap
{
  public:
    ColorMap();
    ColorMap(QString name, std::vector<GLfloat>& data);
    const QString& getName() const { return m_name; };
    const std::vector<GLfloat>& colorMapData() const { return m_colorMapData; };

  private:
    std::vector<GLfloat> m_colorMapData;
    QString m_name;
};

class TransferTexture : public QObject
{
    Q_OBJECT
  public:
    explicit TransferTexture(QObject* parent = nullptr);

    void addColor();
    void removeColor();

    void bind();
    void release();

  public slots:
    void setColorMap(std::vector<GLfloat> cmap);
    void setTransferFunction(TransferFunction tfn);

  private:
    std::vector<GLfloat> m_colorMap;
    TransferFunction m_tfn;
    QOpenGLTexture m_transferTexture;
    bool m_updateNeeded;
};

class IColorMapStore
{
  public:
    virtual ~IColorMapStore() {}
    virtual bool loadColorMapsFromFile(QString path) = 0;
    virtual ColorMap colorMap(const QString& name) const = 0;
    virtual std::vector<QString> availableColorMaps() const = 0;
    virtual void addColorMap(const ColorMap& colorMap) = 0;
};
class ColorMapStore : public IColorMapStore
{
  public:
    ColorMapStore();
    virtual bool loadColorMapsFromFile(QString path);
    virtual ColorMap colorMap(const QString& name) const;
    virtual void addColorMap(const ColorMap& colorMap);
    virtual std::vector<QString> availableColorMaps() const;

  private:
    QMap<QString, ColorMap> m_colorMaps;
    ColorMap m_defaultColorMap;
};

} // namespace tfn
#endif // TRANSFERTEXTURE_H
