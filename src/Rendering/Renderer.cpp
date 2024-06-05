#include "Renderer.hpp"

namespace GravyEngine
{
    RenderSettings::RenderSettings()
    {
        this->wireframe = false;
        this->depthTest = true;
        this->cullFace = true;
        this->alphaBlend = false;
        this->depthFunc = GL_LESS;
    }

    Renderer::Renderer() : Component()
    {

    }

    Renderer::~Renderer()
    {

    }
};