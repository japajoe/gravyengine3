#include "Graphics.hpp"
#include "Shaders/DiffuseShader.hpp"
#include "../External/glad/glad.h"

namespace GravyEngine
{
    void Graphics::Initialize()
    {
        DiffuseShader::Create();
    }

    void Graphics::Deinitialize()
    {
        DiffuseShader::Destroy();
    }

    void Graphics::OnRender()
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
};