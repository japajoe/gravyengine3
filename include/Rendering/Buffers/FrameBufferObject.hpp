#ifndef GRAVYENGINE_FRAMEBUFFEROBJECT_HPP
#define GRAVYENGINE_FRAMEBUFFEROBJECT_HPP

#include "../../Core/Object.hpp"
#include "../../External/glad/glad.h"

namespace GravyEngine
{
    class FrameBufferObject : public Object
    {
    private:
        GLuint id;
    public:
        FrameBufferObject();
        void Generate();
        void Delete();
        void Bind();
        void Unbind();
        GLuint GetId() const;
    };
};

#endif