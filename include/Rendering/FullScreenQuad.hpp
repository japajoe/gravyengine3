#ifndef GRAVYENGINE_FULLSCREENQUAD_HPP
#define GRAVYENGINE_FULLSCREENQUAD_HPP

#include <cstdint>

namespace GravyEngine
{
    class Shader;

    class FullScreenQuad
    {
    public:
        FullScreenQuad();
        void Generate();
        void Delete();
        void Render();
    private:
        uint32_t VAO;
        uint32_t VBO;
        Shader *pShader;
    };
};

#endif