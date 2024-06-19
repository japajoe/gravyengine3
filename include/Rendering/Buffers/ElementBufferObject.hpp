#ifndef GRAVYENGINE_ELEMENTBUFFEROBJECT_HPP
#define GRAVYENGINE_ELEMENTBUFFEROBJECT_HPP

#include "../../Core/Object.hpp"
#include "../../External/glad/glad.h"

namespace GravyEngine
{
    class ElementBufferObject : public Object
    {
    private:
        GLuint id;
    public:
        ElementBufferObject();
        void Generate();
        void Delete();
        void Bind();
        void Unbind();
        void BufferData(GLsizeiptr size, const void *data, GLenum usage);
        void BufferSubData(GLintptr offset, GLsizeiptr size, const void *data);
        GLuint GetId() const;
    };
};

#endif