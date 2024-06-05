#include "Texture.hpp"

namespace GravyEngine
{
    Texture::Texture() : Object()
    {
        id = 0;
        width = 0;
        height = 0;
    }

    Texture::~Texture()
    {
        
    }

    uint32_t Texture::GetId() const
    {
        return id;
    }

    uint32_t Texture::GetWidth() const
    {
        return width;
    }

    uint32_t Texture::GetHeight() const
    {
        return height;
    }

    TextureType Texture::GetType() const
    {
        return type;
    }

    void Texture::Bind(uint32_t unit)
    {

    }

    void Texture::Unbind()
    {

    }

    void Texture::Delete()
    {

    }
};