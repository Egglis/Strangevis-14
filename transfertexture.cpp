#include "transfertexture.h"



TransferTexture::TransferTexture(QObject* parent)
    : QObject(parent), m_transferTexture(QOpenGLTexture::Target1D), m_updateNeeded(false)
{
    // Init??
};

bool TransferTexture::init(){

    
    m_colorMap.push_back(1.0f); // Red
    m_colorMap.push_back(0.0f);
    m_colorMap.push_back(0.0f);
    m_colorMap.push_back(1.0f);
    
    m_colorMap.push_back(0.0f); // Green
    m_colorMap.push_back(1.0f);
    m_colorMap.push_back(0.0f);
    m_colorMap.push_back(0.5f);

    m_colorMap.push_back(0.0f); // Blue
    m_colorMap.push_back(0.0f);
    m_colorMap.push_back(1.0f);
    m_colorMap.push_back(0.0f);
    
    m_updateNeeded = true;
    return true;
};




void TransferTexture::bind(){
    if(m_updateNeeded){

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
        m_transferTexture.setSize(12);
        m_transferTexture.allocateStorage();

        void* data = m_colorMap.data();
        m_transferTexture.setData(QOpenGLTexture::RGBA,QOpenGLTexture::Float32, data);
        
        if(m_transferTexture.isCreated()){
            qDebug() << "Colormap Texture Created";
            m_transferTexture.bind(1);
        }
    }
};


void TransferTexture::release(){
    if(m_transferTexture.isCreated()){
        m_transferTexture.release();
    }
};
