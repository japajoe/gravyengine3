#include "UniformBufferObject.hpp"
#include "../../Core/Debug.hpp"

namespace GravyEngine
{
    std::unordered_map<std::string,UniformBufferObject> UniformBufferObject::buffers;

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

    UniformBufferObject *UniformBufferObject::Add(const std::string &name, const UniformBufferObject &buffer)
    {
        if(buffers.count(name) > 0)
        {
            Debug::WriteError("[UNIFORMBUFFER] can't add %s with ID: %llu because it already exists", name.c_str(), buffer.GetId());
            return nullptr;
        }

        buffers[name] = buffer;

        Debug::WriteLog("[UNIFORMBUFFER] %s added with ID: %llu", name.c_str(), buffer.GetId());

        auto pBuffer = &buffers[name];
        return pBuffer;
    }

    void UniformBufferObject::Remove(const std::string &name)
    {
        auto buffer = Find(name);

        if(buffer)
        {
            Debug::WriteLog("[UNIFORMBUFFER] %s deleted with ID: %llu", name.c_str(), buffer->GetId());
            buffer->Delete();
            buffers.erase(name);
        }
    }

    UniformBufferObject *UniformBufferObject::Find(const std::string &name)
    {
        if(buffers.count(name) == 0)
            return nullptr;
        
        return &buffers[name];
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