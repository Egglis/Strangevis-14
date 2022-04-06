#ifndef APPLICATION_H
#define APPLICATION_H

#include "properties/sharedproperties.h"
#include "texturestore.h"
#include "transfertexture.h"

#include <QApplication>

class StrangevisVisualizerApplication : public QApplication
{
  public:
    StrangevisVisualizerApplication(int argc, char* argv[]);

    const std::shared_ptr<ISharedProperties>& properties() const
    {
        return m_properties;
    };
    std::shared_ptr<ISharedProperties>& properties() { return m_properties; };
    const std::shared_ptr<ITextureStore> textureStore() const
    {
        return m_textureStore;
    };
    std::shared_ptr<ITextureStore> textureStore() { return m_textureStore; };
    const std::shared_ptr<const tfn::IColorMapStore> colorMapStore() const
    {
        return m_colorMapStore;
    };
    std::shared_ptr<tfn::IColorMapStore> colorMapStore()
    {
        return m_colorMapStore;
    };

  private:
    std::shared_ptr<ISharedProperties> m_properties;
    std::shared_ptr<ITextureStore> m_textureStore;
    std::shared_ptr<tfn::IColorMapStore> m_colorMapStore;
};

#endif // APPLICATION_H
