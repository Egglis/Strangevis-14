#include "transfertexture.h"



TransferTexture::TransferTexture(QObject* parent)
    : QObject(parent), m_transferTexture(QOpenGLTexture::Target1D), m_updateNeeded(false)
{
    ColorMap cmap = ColorMap();
    m_colorMap = cmap;
    m_updateNeeded = true;
};


void TransferTexture::setColorMap(ColorMap colormap){
    m_colorMap = colormap;
    m_updateNeeded = true;
};


/*
void TransferTexture::updateA(float value){
    m_colorMap[1] = value;
    m_updateNeeded = true;
}
*/


void TransferTexture::bind(){
    if(m_updateNeeded){
        qDebug() << "Colormap Texture Created";

        if(m_transferTexture.isCreated()){
            m_transferTexture.destroy();
        }
        

        m_transferTexture.setBorderColor(0, 0, 0, 0);
        m_transferTexture.setWrapMode(QOpenGLTexture::ClampToEdge);
        m_transferTexture.setFormat(QOpenGLTexture::RGBA32F);
        m_transferTexture.setMinificationFilter(QOpenGLTexture::Nearest);
        m_transferTexture.setMagnificationFilter(QOpenGLTexture::Nearest);
        m_transferTexture.setAutoMipMapGenerationEnabled(false);
        m_transferTexture.setMipLevels(0);
        m_transferTexture.setSize(256);
        m_transferTexture.allocateStorage();

        m_transferTexture.setData(QOpenGLTexture::RGBA,QOpenGLTexture::Float32, m_colorMap.m_colorMapData);
        
        m_updateNeeded = false;

        if(m_transferTexture.isCreated()){
            m_transferTexture.bind(1);
        }
    }
};


void TransferTexture::release(){
    if(m_transferTexture.isCreated()){
        m_transferTexture.release();
    }
};

ColorMap::ColorMap(QString name, GLfloat data[256][4]) : m_name{name}, m_colorMapData{data[256][4]} 
{};


ColorMap::ColorMap(QString name, std::vector<GLfloat> data) : m_name{name}
{   
    qDebug() << "new Colormap" << name;
    for(int i = 0; i < data.size(); i++){
        qDebug() << data[i];
    }
    for(int i = 0; i < 256; i++){
        for(int j = 0; j < 4; j++){
            m_colorMapData[i][j] = data[(j*256)+i];
        }
    }

};

ColorMap::ColorMap()
{
    // Defualt RGB ColorMap, i know there is a better way, but i am lazy
    m_name = "RGB";
    for(int i = 0; i < 64; i++){
        m_colorMapData[i][0] = 1.0f; // Red
        m_colorMapData[i][1] = 0.0f;
        m_colorMapData[i][2] = 0.0f;
        m_colorMapData[i][3] = (double)(i)/(double)(256);;
    }
        for(int i = 64; i < 128; i++){
        m_colorMapData[i][0] = 0.0f; // Green
        m_colorMapData[i][1] = 1.0f;
        m_colorMapData[i][2] = 0.0f;
        m_colorMapData[i][3] = (double)(i)/(double)(256);;

    }
    for(int i = 128; i < 192; i++){
        m_colorMapData[i][0] = 0.0f; // Blue
        m_colorMapData[i][1] = 0.0f;
        m_colorMapData[i][2] = 1.0f;
        m_colorMapData[i][3] = (double)(i)/(double)(256);;

    }
    for(int i = 192; i < 256; i++){
        m_colorMapData[i][0] = 1.0f; // Yellow
        m_colorMapData[i][1] = 1.0f;
        m_colorMapData[i][2] = 0.0f;
        m_colorMapData[i][3] = (double)(i)/(double)(256);;

    }
}