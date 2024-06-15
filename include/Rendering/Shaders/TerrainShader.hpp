#ifndef GRAVYENGINE_TERRAINSHADER_HPP
#define GRAVYENGINE_TERRAINSHADER_HPP

#include <string>
#include "../Shader.hpp"

namespace GravyEngine
{
    class TerrainShader
    {
    public:
        static Shader *Create();
        static void Destroy();
    };
}

#endif