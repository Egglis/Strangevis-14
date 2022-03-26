#ifndef TRANSFERTEXTURE_H
#define TRANSFERTEXTURE_H

#include <QColor>
#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
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
    void setColorMap(ColorMap cmap);
    ColorMap getColorMap() { return m_colorMap; };

    void bind();
    void release();

  private:
    ColorMap m_colorMap;
    QOpenGLTexture m_transferTexture;
    bool m_updateNeeded;
};
std::vector<ColorMap> loadColorMapsFromFile();

} // namespace tfn
#endif // TRANSFERTEXTURE_H
