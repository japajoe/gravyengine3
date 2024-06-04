#include "MeshRenderer.hpp"
#include "../External/glad/glad.h"

namespace GravyEngine
{
    MeshRenderer::MeshRenderer() : Renderer()
    {
        pMesh = nullptr;
        SetName("MeshRenderer");
    }

    void MeshRenderer::SetMesh(Mesh *mesh)
    {
        this->pMesh = mesh;
    }

    Mesh *MeshRenderer::GetMesh() const
    {
        return pMesh;
    }

    void MeshRenderer::OnRender()
    {
        if(!pMesh)
            return;

        if(!pMaterial)
            return;

        pMaterial->UpdateUniforms();
        pMaterial->Use();
    }
};