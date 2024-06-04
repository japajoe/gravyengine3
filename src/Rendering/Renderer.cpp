#include "Renderer.hpp"

namespace GravyEngine
{
    Renderer::Renderer() : Component()
    {
        this->pMaterial = nullptr;
    }

    Renderer::~Renderer()
    {

    }

    void Renderer::SetMaterial(Material *material)
    {
        this->pMaterial = material;
    }

    Material *Renderer::GetMaterial() const
    {
        return pMaterial;
    }
};