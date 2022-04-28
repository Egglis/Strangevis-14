#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QVector2D>

class ViewPort
{
  public:
    ViewPort(int w, int h) : m_width{w}, m_height{h} {};
    QVector2D viewPort() const { return QVector2D{static_cast<float>(m_width), static_cast<float>(m_height)}; };
    float aspectRatio() const
    {
        return static_cast<float>(m_width) / m_height;
    };
    void updateViewPort(int w, int h)
    {
        m_width = w;
        m_height = h;
    };

  private:
    int m_width;
    int m_height;
};

#endif // VIEWPORT_H
