#ifndef GRAVYENGINE_VERTEXBUFFEROBJECT_HPP
#define GRAVYENGINE_VERTEXBUFFEROBJECT_HPP

#include "../../Core/Object.hpp"
#include "../../External/glad/glad.h"

namespace GravyEngine
{
    class VertexBufferObject : public Object
    {
    private:
        GLuint id;
    public:
        VertexBufferObject();
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