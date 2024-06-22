#ifndef GRAVYENGINE_TEXTURE2DARRAY_HPP
#define GRAVYENGINE_TEXTURE2DARRAY_HPP

#include "Texture.hpp"
#include "../External/glad/glad.h"

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
    private:
        uint32_t depth;
    };
};

#endif