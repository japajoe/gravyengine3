#include "GL.hpp"

namespace GravyEngine
{
    GLenum GL::lastDepthFunc = GL_LESS;
    GLboolean GL::lastDepthTest = GL_TRUE;
    GLboolean GL::lastCullFace = GL_TRUE;
    GLenum GL::lastPolygonMode = GL_FILL;
    GLboolean GL::lastBlendMode = GL_FALSE;
    GLboolean GL::lastDepthMask = GL_TRUE;

    void GL::SetDepthFunc(GLenum func)
    {
        if (func != lastDepthFunc)
        {
            glDepthFunc(func);
            lastDepthFunc = func;
        }
    }

    void GL::EnableDepthTest()
    {
        if (!lastDepthTest)
        {
            glEnable(GL_DEPTH_TEST);
            lastDepthTest = GL_TRUE;
        }
    }

    void GL::DisableDepthTest()
    {
        if (lastDepthTest)
        {
            glDisable(GL_DEPTH_TEST);
            lastDepthTest = GL_FALSE;
        }
    }

    void GL::EnableCullFace()
    {
        if (!lastCullFace)
        {
            glEnable(GL_CULL_FACE);
            lastCullFace = GL_TRUE;
        }
    }

    void GL::DisableCullFace()
    {
        if (lastCullFace)
        {
            glDisable(GL_CULL_FACE);
            lastCullFace = GL_FALSE;
        }
    }

    void GL::EnableBlendMode()
    {
        if(!lastBlendMode)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            lastBlendMode = GL_TRUE;
        }
    }

    void GL::DisableBlendMode()
    {
        if(lastBlendMode)
        {
            glDisable(GL_BLEND);
            lastBlendMode = GL_FALSE;
        }
    }

    void GL::SetPolygonMode(GLenum mode)
    {
        if (mode != lastPolygonMode)
        {
            glPolygonMode(GL_FRONT_AND_BACK, mode);
            lastPolygonMode = mode;
        }
    }

    void GL::EnableDepthMask()
    {
        if (!lastDepthMask)
        {
            glDepthMask(GL_TRUE);
            lastDepthMask = GL_TRUE;
        }
    }

    void GL::DisableDepthMask()
    {
        if (lastDepthMask)
        {
            glDepthMask(GL_FALSE);
            lastDepthMask = GL_FALSE;
        }
    }
};