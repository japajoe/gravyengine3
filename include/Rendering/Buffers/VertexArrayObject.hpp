#ifndef GRAVYENGINE_VERTEXARRAYOBJECT_HPP
#define GRAVYENGINE_VERTEXARRAYOBJECT_HPP

#include "../../Core/Object.hpp"
#include "../../External/glad/glad.h"

namespace GravyEngine
{
    class VertexArrayObject : public Object
    {
    private:
        GLuint id;
    public:
        VertexArrayObject();
        void Generate();
        void Delete();
        void Bind();
        void Unbind();
        void EnableVertexAttribArray(GLuint index);
        void VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
        void VertexAttribDivisor(GLuint index, GLuint divisor);
        GLuint GetId() const;
    };
};

#endif