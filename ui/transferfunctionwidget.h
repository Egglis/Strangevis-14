#ifndef TRANSFER_WIDGET_H
#define TRANSFER_WIDGET_H

#include "../transfertexture.h"

#include <QWidget>
#include <QGridLayout>
#include <QOpenGLTexture>
#include <QSlider>
#include <QComboBox>


class SharedProperties;
/*
TODO:
Add DataPoints
Add Colormaps 
Add more settings

*/
class ColorMapSelector : public QWidget
{
        Q_OBJECT
    public:
        ColorMapSelector(QWidget* parent, std::vector<ColorMap> options);
        int getSelectedItem() {return m_selectedItem; };
    public slots:
        void setIndex(int index);
    signals:
        void valueChanged(int index);
    private:
        QComboBox* m_box;
        int m_selectedItem;
};


class TransferWidget : public QWidget {
        Q_OBJECT
    public:
        TransferWidget(const std::shared_ptr<SharedProperties>& properties, QWidget* parent);
    public slots:
        void setSelectedColorMap(int index);
    signals:
        void valueChanged(ColorMap cmap);
    private:
        bool loadColorMaps();

        const std::shared_ptr<SharedProperties>& m_properties;
        QGridLayout m_layout;
        int m_index;
        ColorMapSelector* m_selector;
        std::vector<ColorMap> m_colorMaps;
};



#endif // TRANSFER_WIDGET_H