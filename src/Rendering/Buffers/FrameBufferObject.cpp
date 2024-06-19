#include "FrameBufferObject.hpp"

namespace GravyEngine
{
    FrameBufferObject::FrameBufferObject()
    {
        this->id = 0;
    }

    void FrameBufferObject::Generate()
    {
        glGenFramebuffers(1, &id);
    }

    void FrameBufferObject::Delete()
    {
        if(id > 0)
        {
            glDeleteFramebuffers(1, &id);
            id = 0;
        }
    }

    void FrameBufferObject::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, id);
    }

    void FrameBufferObject::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLuint FrameBufferObject::GetId() const
    {
        return id;
    }
};