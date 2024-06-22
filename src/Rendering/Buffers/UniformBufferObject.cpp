#include "UniformBufferObject.hpp"
#include "../../Core/Debug.hpp"

namespace GravyEngine
{
    UniformBufferObject::UniformBufferObject(): Object()
    {
        this->id = 0;
    }

    UniformBufferObject::UniformBufferObject(uint32_t bindingIndex, size_t bufferSize)
    {
        this->id = 0;
        Generate();
        Bind();
        BufferData(bufferSize, nullptr, GL_DYNAMIC_DRAW);
        BindBufferBase(bindingIndex);
        Unbind();
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
            id = 0;
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

    void UniformBufferObject::BindToShader(GLuint program, GLuint uniformBlockBinding, const std::string &uniformBlockName)
    {
        unsigned int uniformBlockIndex = glGetUniformBlockIndex(program, uniformBlockName.c_str());
        glUniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
    }

    GLuint UniformBufferObject::GetId() const
    {
        return id;
    }
};