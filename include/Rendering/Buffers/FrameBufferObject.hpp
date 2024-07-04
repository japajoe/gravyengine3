#ifndef GRAVYENGINE_FRAMEBUFFEROBJECT_HPP
#define GRAVYENGINE_FRAMEBUFFEROBJECT_HPP

#include "../../Core/Object.hpp"
#include "../../External/glad/glad.h"
#include <cstdint>

namespace GravyEngine
{
    class FrameBufferObject : public Object
    {
    public:
        FrameBufferObject();
        FrameBufferObject(uint32_t width, uint32_t height);
        void Generate();
        void Delete();
        void Bind();
        void Unbind();
        void Resize(uint32_t width, uint32_t height);
        GLuint GetId() const;
        GLuint GetTextureId() const;
    private:
        GLuint id;
        GLuint textureAttachmentId;
        GLuint depthAttachmentId;
        uint32_t width;
        uint32_t height;
    };
};

#endif