#ifndef GRAVYENGINE_UNIFORMBUFFEROBJECT_HPP
#define GRAVYENGINE_UNIFORMBUFFEROBJECT_HPP

#include "../../External/glad/glad.h"
#include "../../Core/Object.hpp"
#include <string>
#include <unordered_map>

namespace GravyEngine
{
    class UniformBufferObject : public Object
    {
    public:
        UniformBufferObject();
        void Generate();
        void Delete();
        void Bind();
        void Unbind();
        void BindBufferBase(GLuint index);
        void BufferData(GLsizeiptr size, const void *data, GLenum usage);
        void BufferSubData(GLintptr offset, GLsizeiptr size, const void *data);
        //void UniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
        //void BindToShader(GLuint program, GLuint uniformBlockBinding, const std::string &uniformBlockName);
        //GLuint GetUniformBlockIndex(GLuint program, const std::string &uniformBlockName);
        GLuint GetId() const;
        static UniformBufferObject *Add(const std::string &name, const UniformBufferObject &buffer);
        static void Remove(const std::string &name);
        static UniformBufferObject *Find(const std::string &name);
    private:
        GLuint id;
        static std::unordered_map<std::string,UniformBufferObject> buffers;
    };
};

#endif