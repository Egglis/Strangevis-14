#ifndef TEXTURESTORE_H
#define TEXTURESTORE_H

#include "transfertexture.h"
#include "volume.h"

#include <QObject>

class ITextureStore
{
  public:
    virtual ~ITextureStore() {}

    virtual Volume& volume() = 0;
    virtual const Volume& volume() const = 0;

    virtual tfn::TransferTexture& transferFunction() = 0;
    virtual const tfn::TransferTexture& transferFunction() const = 0;
};
class TextureStore : public QObject, public ITextureStore
{
    Q_OBJECT
  public:
    explicit TextureStore(QObject* parent = nullptr);

    virtual Volume& volume() { return m_volume; };
    virtual const Volume& volume() const { return m_volume; };

    virtual tfn::TransferTexture& transferFunction() { return m_transfertexture; };
    virtual const tfn::TransferTexture& transferFunction() const
    {
        return m_transfertexture;
    };

  private:
    Volume m_volume;
    tfn::TransferTexture m_transfertexture;
};

#endif // TEXTURESTORE_H
