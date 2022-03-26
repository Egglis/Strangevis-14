#ifndef TRANSFERTEXTURE_H
#define TRANSFERTEXTURE_H

#include <QColor>
#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QMap>
#include <vector>

namespace tfn
{
namespace size
{
constexpr static int NumPoints = 256;
constexpr static int NumChannels = 3;
constexpr static int ArraySize = NumPoints * NumChannels;
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
    // std::vector<GLfloat> getColorMap() { return m_colorMap; };

    void bind();
    void release();

  public slots:
    void setColorMap(std::vector<GLfloat> cmap);

  private:
    std::vector<GLfloat> m_colorMap;
    QOpenGLTexture m_transferTexture;
    bool m_updateNeeded;
};

class IColorMapStore
{
  public:
  virtual ~IColorMapStore(){}
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
