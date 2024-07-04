#include "FrameBufferObject.hpp"
#include "../../Core/Debug.hpp"
#include "../../Core/Screen.hpp"

namespace GravyEngine
{
    FrameBufferObject::FrameBufferObject()
    {
        this->id = 0;
        this->textureAttachmentId = 0;
        this->depthAttachmentId = 0;
        this->width = 0;
        this->height = 0;
    }

    FrameBufferObject::FrameBufferObject(uint32_t width, uint32_t height)
    {
        this->id = 0;
        this->textureAttachmentId = 0;
        this->depthAttachmentId = 0;
        this->width = width;
        this->height = height;
    }

    void FrameBufferObject::Generate()
    {
        if(width == 0 || height == 0)
            return;

        glGenFramebuffers(1, &id);
        glBindFramebuffer(GL_FRAMEBUFFER, id);

        glGenTextures(1, &textureAttachmentId);
        glBindTexture(GL_TEXTURE_2D, textureAttachmentId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureAttachmentId, 0);

        glGenRenderbuffers(1, &depthAttachmentId);
        glBindRenderbuffer(GL_RENDERBUFFER, depthAttachmentId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthAttachmentId);

        uint32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        
        if(status != GL_FRAMEBUFFER_COMPLETE)
        {
            Debug::WriteError("Failed to initialize frame buffer, status: %llu", status);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void FrameBufferObject::Delete()
    {
        if(id > 0)
        {
            glDeleteFramebuffers(1, &id);
            id = 0;
            textureAttachmentId = 0;
            depthAttachmentId = 0;
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

    void FrameBufferObject::Resize(uint32_t width, uint32_t height)
    {
        glBindTexture(GL_TEXTURE_2D, textureAttachmentId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        glBindFramebuffer(GL_FRAMEBUFFER, id);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureAttachmentId, 0); 

        glBindRenderbuffer(GL_RENDERBUFFER, depthAttachmentId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthAttachmentId);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    GLuint FrameBufferObject::GetId() const
    {
        return id;
    }

    GLuint FrameBufferObject::GetTextureId() const
    {
        return textureAttachmentId;
    }
};