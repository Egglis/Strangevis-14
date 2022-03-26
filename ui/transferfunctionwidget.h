#ifndef TRANSFERFUNCTIONWIDGET_H
#define TRANSFERFUNCTIONWIDGET_H

#include "../transfertexture.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QOpenGLTexture>
#include <QSlider>
#include <QComboBox>


class ISharedProperties;
/*
TODO:
Add DataPoints
Add Colormaps
Add more settings

*/
class ColorMapSelector : public QComboBox
{
        Q_OBJECT
    public:
        ColorMapSelector(QWidget* parent, std::vector<ColorMap> options);
};


class TransferWidget : public QWidget {
        Q_OBJECT
    public:
        TransferWidget(const std::shared_ptr<ISharedProperties> properties, QWidget* parent);
    public slots:
        void setSelectedColorMap(int index);
    signals:
        void valueChanged(ColorMap cmap);
    private:
        const std::shared_ptr<ISharedProperties> m_properties;
        QHBoxLayout* m_layout;
        int m_index;
        ColorMapSelector* m_selector;
        std::vector<ColorMap> m_colorMaps;
};



#endif // TRANSFERFUNCTIONWIDGET_H
