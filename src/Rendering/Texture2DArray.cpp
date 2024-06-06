#include "Texture2DArray.hpp"
#include "../Core/Debug.hpp"

namespace GravyEngine
{
    std::unordered_map<std::string,Texture2DArray> Texture2DArray::textures;

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
        }
    }

    uint32_t Texture2DArray::GetDepth() const
    {
        return depth;
    }

    Texture2DArray *Texture2DArray::Add(const std::string &name, const Texture2DArray &texture)
    {
        if(textures.count(name) > 0)
        {
            Debug::WriteError("[TEXTURE] can't add %s with ID: %llu because it already exists", name.c_str(), texture.GetId());
            return nullptr;
        }

        textures[name] = texture;

        Debug::WriteLog("[TEXTURE] %s added with ID: %llu", name.c_str(), texture.GetId());

        auto pTexture = &textures[name];
        return pTexture;
    }

    void Texture2DArray::Remove(const std::string &name)
    {
        auto texture = Find(name);

        if(texture)
        {
            Debug::WriteLog("[TEXTURE] %s deleted with ID: %llu", name.c_str(), texture->GetId());
            texture->Delete();
            textures.erase(name);
        }
    }

    Texture2DArray *Texture2DArray::Find(const std::string &name)
    {
        if(textures.count(name) == 0)
            return nullptr;
        
        return &textures[name];
    }
};