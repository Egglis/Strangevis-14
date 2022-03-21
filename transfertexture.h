#ifndef TRANSFER_TEXTURE_H
#define TRANSFER_TEXTURE_H

#include <QOpenGLFunctions>
#include <QObject>
#include <QOpenGLTexture>
#include <vector>
#include <QColor>



class TransferTexture : public QObject {
        Q_OBJECT
    public:
        explicit TransferTexture(QObject* parent = nullptr);
        bool init();

        void addColor();
        void removeColor();

        void bind();
        void release();
    private:
        std::vector<GLfloat> m_colorMap;
        QOpenGLTexture m_transferTexture;
        bool m_updateNeeded;
};



#endif