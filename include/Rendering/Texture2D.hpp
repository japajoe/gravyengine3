#ifndef GRAVYENGINE_TEXTURE2D_HPP
#define GRAVYENGINE_TEXTURE2D_HPP

#include "Texture.hpp"
#include "../External/glad/glad.h"
#include "../System/Drawing/Color.hpp"
#include "../System/Drawing/Image.hpp"
#include <unordered_map>

namespace GravyEngine
{
    class Texture2D : public Texture
    {
    public:
        Texture2D();
        Texture2D(Image *image);
        Texture2D(uint32_t width, uint32_t height, const Color &color);
        Texture2D(uint32_t id, uint32_t width, uint32_t height);
        void Bind(GLuint unit) override;
        void Unbind() override;
        void Delete() override;
        static Texture2D *Add(const std::string &name, const Texture2D &texture);
        static void Remove(const std::string &name);
        static void RemoveAll();
        static Texture2D *Find(const std::string &name);
    private:
        static std::unordered_map<std::string,Texture2D> textures;
    };
};

#endif