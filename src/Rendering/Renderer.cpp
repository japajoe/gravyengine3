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

    void Renderer::SetMaterial(const std::shared_ptr<Material> &material)
    {
        this->pMaterial = material;
    }
};