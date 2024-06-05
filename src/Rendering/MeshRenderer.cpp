#include "MeshRenderer.hpp"
#include "../Core/Camera.hpp"
#include "../Core/Transform.hpp"
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

        if(!pMaterial->GetShader())
            return;

        pMaterial->Use(GetTransform(), Camera::GetMain());

        pMesh->GetVAO()->Bind();

        if(pMesh->GetEBO()->GetId() > 0)
            glDrawElements(GL_TRIANGLES, pMesh->GetIndicesCount(), GL_UNSIGNED_INT, 0);
        else
            glDrawArrays(GL_TRIANGLES, 0, pMesh->GetVerticesCount());

        pMesh->GetVAO()->Unbind();
    }
};