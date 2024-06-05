#ifndef GRAVYENGINE_GL_HPP
#define GRAVYENGINE_GL_HPP

#include "../External/glad/glad.h"

namespace GravyEngine
{
    class GL
    {
    public:
        static void SetDepthFunc(GLenum func);
        static void EnableDepthTest();
        static void DisableDepthTest();
        static void EnableCullFace();
        static void DisableCullFace();
        static void EnableBlendMode();
        static void DisableBlendMode();
        static void SetPolygonMode(GLenum mode);
        static void EnableDepthMask();
        static void DisableDepthMask();
    private:
        static GLenum lastDepthFunc;
        static GLboolean lastDepthTest;
        static GLboolean lastCullFace;
        static GLenum lastPolygonMode;
        static GLboolean lastBlendMode;
        static GLboolean lastDepthMask;
    };
};

#endif