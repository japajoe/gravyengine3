#include "UniformBufferObject.hpp"

namespace GravyEngine
{
    UniformBufferObject::UniformBufferObject(): Object()
    {
        this->id = 0;
    }

    void UniformBufferObject::Generate()
    {
        glGenBuffers(1, &id);
    }

    void UniformBufferObject::Delete()
    {
        if(id > 0)
        {
            glDeleteBuffers(1, &id);
        }
    }

    void UniformBufferObject::Bind()
    {
        glBindBuffer(GL_UNIFORM_BUFFER, id);
    }

    void UniformBufferObject::Unbind()
    {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void UniformBufferObject::BindBufferBase(GLuint index)
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, index, id);
    }

    void UniformBufferObject::BufferData(GLsizeiptr size, const void *data, GLenum usage)
    {
        glBufferData(GL_UNIFORM_BUFFER, size, data, usage);
    }

    void UniformBufferObject::BufferSubData(GLintptr offset, GLsizeiptr size, const void *data)
    {
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    }

    GLuint UniformBufferObject::GetId() const
    {
        return id;
    }

    // UniformBufferObject::UniformBufferObject() : Object()
    // {
    //     id = 0;
    // }

    // void UniformBufferObject::Generate()
    // {
    //     glGenBuffers(1, &id);
    // }

    // void UniformBufferObject::Delete()
    // {
    //     if(id > 0)
    //     {
    //         glDeleteBuffers(1, &id);
    //     }
    // }

    // void UniformBufferObject::Bind()
    // {
    //     glBindBuffer(GL_UNIFORM_BUFFER, id);
    // }

    // void UniformBufferObject::Unbind()
    // {
    //     glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // }

    // void UniformBufferObject::BindBufferBase(GLuint index)
    // {
    //     glBindBufferBase(GL_UNIFORM_BUFFER, index, id);
    // }

    // void UniformBufferObject::BufferData(GLsizeiptr size, const void *data, GLenum usage)
    // {
    //     glBufferData(GL_UNIFORM_BUFFER, size, data, usage);
    // }

    // void UniformBufferObject::BufferSubData(GLintptr offset, GLsizeiptr size, const void *data)
    // {
    //     glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    // }

    // void UniformBufferObject::UniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding)
    // {
    //     glUniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
    // }

    // GLuint UniformBufferObject::GetUniformBlockIndex(GLuint program, const std::string &uniformBlockName)
    // {
    //     return glGetUniformBlockIndex(program, uniformBlockName.c_str());
    // }

    // void UniformBufferObject::BindToShader(GLuint program, GLuint uniformBlockBinding, const std::string &uniformBlockName)
    // {
    //     GLuint uboIndex = glGetUniformBlockIndex(program, uniformBlockName.c_str());
    //     glUniformBlockBinding(program, uboIndex, uniformBlockBinding);
    // }

    // GLuint UniformBufferObject::GetId() const
    // {
    //     return id;
    // }
};