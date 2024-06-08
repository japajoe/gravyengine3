#ifndef GRAVYENGINE_LINESHADER_HPP
#define GRAVYENGINE_LINESHADER_HPP

#include <string>
#include "../Shader.hpp"

namespace GravyEngine
{
    class LineShader
    {
    public:
        static Shader *Create();
        static void Destroy();
    };
}

#endif