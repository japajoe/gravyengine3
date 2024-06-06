#ifndef GRAVYENGINE_TEXTURE2DARRAY_HPP
#define GRAVYENGINE_TEXTURE2DARRAY_HPP

#include "Texture.hpp"
#include "../External/glad/glad.h"
#include <unordered_map>

namespace GravyEngine
{
    class Texture2DArray : public Texture
    {
    public:
        Texture2DArray();
        Texture2DArray(uint32_t width, uint32_t height, uint32_t depth);
        void Bind(GLuint unit) override;
        void Unbind() override;
        void Delete() override;
        uint32_t GetDepth() const;
        static Texture2DArray *Add(const std::string &name, const Texture2DArray &texture);
        static void Remove(const std::string &name);
        static Texture2DArray *Find(const std::string &name);
    private:
        uint32_t depth;
        static std::unordered_map<std::string,Texture2DArray> textures;
    };
};

#endif