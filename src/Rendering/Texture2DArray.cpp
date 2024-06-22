#include "Texture2DArray.hpp"
#include "../Core/Debug.hpp"

namespace GravyEngine
{
    Texture2DArray::Texture2DArray() : Texture()
    {
        this->type = TextureType::Texture2DArray;
        this->depth = 1;
    }

    Texture2DArray::Texture2DArray(uint32_t width, uint32_t height, uint32_t depth) : Texture()
    {
        this->type = TextureType::Texture2DArray;
        this->width = width;
        this->height = height;
        this->depth = depth;

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D_ARRAY, id);
        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, width, height, depth, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        constexpr float bordercolor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, bordercolor);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }

    void Texture2DArray::Bind(GLuint unit)
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D_ARRAY, id);
    }

    void Texture2DArray::Unbind()
    {
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }

    void Texture2DArray::Delete()
    {
        if(id > 0)
        {
            glDeleteTextures(1, &id);
            id = 0;
        }
    }

    uint32_t Texture2DArray::GetDepth() const
    {
        return depth;
    }
};