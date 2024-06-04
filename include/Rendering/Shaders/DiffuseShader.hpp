#ifndef DIFFUSESHADER_HPP
#define DIFFUSESHADER_HPP

#include <string>
#include "../Shader.hpp"

namespace GravyEngine
{
    class DiffuseShader
    {
    public:
        static Shader *Create();
        static void Destroy();
    };
}

#endif