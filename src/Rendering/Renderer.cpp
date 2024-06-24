#include "Renderer.hpp"
#include "Graphics.hpp"

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
        renderOrder = 1000;

        //Maybe move this to OnInitialize?
        Graphics::AddRenderer(this);
    }

    void Renderer::OnDestroy()
    {
        Graphics::RemoveRenderer(this);
    }

    Mesh *Renderer::GetMesh(size_t index) const
    {
        return nullptr;
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

    void Renderer::SetRenderOrder(uint32_t order)
    {
        this->renderOrder = order;
    }

    uint32_t Renderer::GetRenderOrder() const
    {
        return renderOrder;
    }
};