#ifndef TRANSFER_TEXTURE_H
#define TRANSFER_TEXTURE_H

#include <QOpenGLFunctions>
#include <QObject>
#include <QOpenGLTexture>
#include <vector>
#include <QColor>

class ColorMap 
{
    public:
        ColorMap();
        ColorMap(QString name, std::vector<GLfloat> data);
        ColorMap(QString name, GLfloat data[256][4]);
        QString getName() const { return m_name; };
        int getSize() const { return m_size; };
        GLfloat m_colorMapData[256][4];
    private:
       QString m_name;
       int m_size;
};


class TransferTexture : public QObject {
        Q_OBJECT
    public:
        explicit TransferTexture(QObject* parent = nullptr);

        void addColor();
        void removeColor();
        void updateA(float value);
        void setColorMap(ColorMap colormap);
        ColorMap getColorMap() {return m_colorMap;};

        void bind();
        void release();
    private:
        ColorMap m_colorMap;
        QOpenGLTexture m_transferTexture;
        bool m_updateNeeded;
};


#endif