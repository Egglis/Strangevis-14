#ifndef TRANSFER_TEXTURE_H
#define TRANSFER_TEXTURE_H

#include <QColor>
#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <vector>


class ColorMap
{
  public:
    ColorMap();
    ColorMap(QString name, std::vector<GLfloat>& data);
    QString getName() const { return m_name; };
    int getSize() const { return m_size; };
    std::vector<GLfloat> m_colorMapData;

  private:
    QString m_name;
    int m_size;
};

class TransferTexture : public QObject
{
    Q_OBJECT
  public:
    explicit TransferTexture(QObject* parent = nullptr);

    void addColor();
    void removeColor();
    ColorMap getColorMap() { return m_colorMap; };

    void bind();
    void release();

  private:
    ColorMap m_colorMap;
    QOpenGLTexture m_transferTexture;
    bool m_updateNeeded;
};

#endif
