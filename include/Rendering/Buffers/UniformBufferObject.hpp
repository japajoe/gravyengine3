#ifndef GRAVYENGINE_UNIFORMBUFFEROBJECT_HPP
#define GRAVYENGINE_UNIFORMBUFFEROBJECT_HPP

#include "../../External/glad/glad.h"
#include "../../Core/Object.hpp"
#include <string>
#include <cstdlib>
#include <cstdint>

namespace GravyEngine
{
    class UniformBufferObject : public Object
    {
    public:
        UniformBufferObject();
        UniformBufferObject(uint32_t bindingIndex, size_t bufferSize);
        void Generate();
        void Delete();
        void Bind();
        void Unbind();
        void BindBufferBase(GLuint index);
        void BufferData(GLsizeiptr size, const void *data, GLenum usage);
        void BufferSubData(GLintptr offset, GLsizeiptr size, const void *data);
        void BindToShader(GLuint program, GLuint uniformBlockBinding, const std::string &uniformBlockName);
        GLuint GetId() const;
    private:
        GLuint id;
    };
};

#endif