#include "Texture2D.hpp"
#include "../System/Mathf.hpp"
#include "../Core/Debug.hpp"

namespace GravyEngine
{
    Texture2D::Texture2D() : Texture()
    {
        this->id = 0;
        this->type = TextureType::Texture2D;
    }

    Texture2D::Texture2D(Image *image) : Texture()
    {
        this->id = 0;
        this->type = TextureType::Texture2D;

        uint8_t *data = image->GetData();

        if(data)
        {
            width = image->GetWidth();
            height = image->GetHeight();

            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            //LinearMipmapLinear

            uint32_t channels = image->GetChannels();

            switch(channels)
            {
                case 1:
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->GetWidth(), image->GetHeight(), 0, GL_RED, GL_UNSIGNED_BYTE, data);    
                    break;
                }
                case 2:
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->GetWidth(), image->GetHeight(), 0, GL_RG, GL_UNSIGNED_BYTE, data);
                    break;
                }
                case 3:
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->GetWidth(), image->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                    break;
                }
                case 4:
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->GetWidth(), image->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                    break;
                }
                default:
                {
                    glBindTexture(GL_TEXTURE_2D, 0);
                    glDeleteTextures(1, &id);
                    std::string error = "Failed to load texture: Unsupported number of channels: " + std::to_string(image->GetChannels());
                    throw TextureException(error.c_str());
                    break;
                }
            }
            
            glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
        } 
        else 
        {
            throw TextureException("Failed to load texture: No valid data passed");
        }
    }

    Texture2D::Texture2D(uint32_t width, uint32_t height, const Color &color) : Texture()
    {
        this->id = 0;
        this->width = 0;
        this->height = 0;
        this->type = TextureType::Texture2D;

        if(width == 0 || height == 0)
            throw TextureException("Failed to load texture: Texture width and height must be greater than 0");

        uint32_t channels = 4;
        uint32_t size = width * height * channels;
        uint8_t *data = new uint8_t[size];

        if(data)
        {
            this->width = width;
            this->height = height;

            for(size_t i = 0; i < size; i += channels)
            {
                uint8_t r = static_cast<uint8_t>(Mathf::Clamp(color.r * 255, 0.0, 255.0));
                uint8_t g = static_cast<uint8_t>(Mathf::Clamp(color.g * 255, 0.0, 255.0));
                uint8_t b = static_cast<uint8_t>(Mathf::Clamp(color.b * 255, 0.0, 255.0));
                uint8_t a = static_cast<uint8_t>(Mathf::Clamp(color.a * 255, 0.0, 255.0));

                data[i+0] = r;
                data[i+1] = g;
                data[i+2] = b;
                data[i+3] = a;
            }

            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            
            glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);

            delete[] data;
        } 
        else 
        {
            throw TextureException("Failed to load texture: Failed to allocate memory");
        }
    }

    Texture2D::Texture2D(uint32_t id, uint32_t width, uint32_t height) : Texture()
    {
        this->id = id;
        this->width = width;
        this->height = height;
        this->type = TextureType::Texture2D;
    }

    void Texture2D::Bind(GLuint unit)
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void Texture2D::Unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture2D::Delete()
    {
        if(id > 0)
        {
            glDeleteTextures(1, &id);
            id = 0;
        }
    }
};