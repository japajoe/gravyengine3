#ifndef GRAVYENGINE_PARTICLESHADER_HPP
#define GRAVYENGINE_PARTICLESHADER_HPP

#include <string>
#include "../Shader.hpp"

namespace GravyEngine
{
    class ParticleShader
    {
    public:
        static Shader *Create();
        static void Destroy();
    };
}

#endif