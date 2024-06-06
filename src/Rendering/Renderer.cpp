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
        castShadows = true;
    }

    Renderer::~Renderer()
    {

    }

    void Renderer::SetCastShadows(bool castShadows)
    {
        this->castShadows = castShadows;
    }

    bool Renderer::GetCastShadows() const
    {
        return castShadows;
    }

    void Renderer::SetReceiveShadows(bool receiveShadows)
    {
        this->receiveShadows = receiveShadows;
    }

    bool Renderer::GetReceiveShadows() const
    {
        return receiveShadows;
    }
};