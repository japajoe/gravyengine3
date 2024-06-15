#include "MeshRenderer.hpp"
#include "../Core/Camera.hpp"
#include "../Core/GameObject.hpp"
#include "../Core/Transform.hpp"
#include "../External/glad/glad.h"
#include "GL.hpp"

namespace GravyEngine
{
    MeshRendererData::MeshRendererData(Mesh *mesh, const std::shared_ptr<Material> &material)
    {
        this->pMesh = mesh;
        this->pMaterial = material;
    }

    MeshRenderer::MeshRenderer() : Renderer()
    {
        SetName("MeshRenderer");
    }

    void MeshRenderer::Add(Mesh *mesh, const std::shared_ptr<Material> &material)
    {
        data.push_back(MeshRendererData(mesh, material));
    }

    void MeshRenderer::Remove(size_t index)
    {
        if(data.size() == 0)
            return;
        if(index >= data.size())
            return;
        data.erase(data.begin() + index);
    }

    void MeshRenderer::SetMesh(Mesh *mesh, size_t index)
    {
        if(data.size() == 0)
            return;
        if(index >= data.size())
            return;
        data[index].pMesh = mesh;
    }

    Mesh *MeshRenderer::GetMesh(size_t index) const
    {
        if(data.size() == 0)
            return nullptr;
        if(index >= data.size())
            return nullptr;
        return data[index].pMesh;
    }

    RenderSettings *MeshRenderer::GetSettings(size_t index)
    {
        if(data.size() == 0)
            return nullptr;
        if(index >= data.size())
            return nullptr;
        return &data[index].settings;
    }

    void MeshRenderer::SetMaterial(const std::shared_ptr<Material> &material, size_t index)
    {
        if(data.size() == 0)
            return;
        if(index >= data.size())
            return;
        data[index].pMaterial = material;
    }

    void MeshRenderer::OnRender()
    {
        if(!GetGameObject()->GetIsActive())
            return;

        if(data.size() == 0)
            return;

        Camera *camera = Camera::GetMain();
        Transform *transform = GetTransform();

        if(!camera || !transform)
            return;

        for(size_t i = 0; i < data.size(); i++)
        {
            Mesh *pMesh = data[i].pMesh;

            if(!pMesh)
                return;

            if(pMesh->GetVAO()->GetId() == 0)
                return;

            Material *pMaterial = data[i].pMaterial.get();

            if(!pMaterial)
                continue;

            if(!pMaterial->GetShader())
                continue;

            auto &settings = data[i].settings;

            if(settings.depthTest)
                GL::EnableDepthTest();
            else
                GL::DisableDepthTest();
            
            if(settings.cullFace)
                GL::EnableCullFace();
            else
                GL::DisableCullFace();
            
            if(settings.alphaBlend)
                GL::EnableBlendMode();
            else
                GL::DisableBlendMode();

            GL::SetDepthFunc(settings.depthFunc);

            pMaterial->Use(transform, camera);

            pMesh->GetVAO()->Bind();

            if(pMesh->GetEBO()->GetId() > 0)
                glDrawElements(GL_TRIANGLES, pMesh->GetIndicesCount(), GL_UNSIGNED_INT, 0);
            else
                glDrawArrays(GL_TRIANGLES, 0, pMesh->GetVerticesCount());

            pMesh->GetVAO()->Unbind();
        }
    }

    void MeshRenderer::OnRender(Material *material, Camera *camera)
    {
        if(!GetGameObject()->GetIsActive())
            return;

        if(data.size() == 0)
            return;

        Transform *transform = GetTransform();

        if(!material || !camera || !transform)
            return;

        if(!material->GetShader())
            return;

        for(size_t i = 0; i < data.size(); i++)
        {
            Mesh *pMesh = data[i].pMesh;

            if(!pMesh)
                continue;

            if(pMesh->GetVAO()->GetId() == 0)
                continue;

            auto &settings = data[i].settings;

            if(settings.depthTest)
                GL::EnableDepthTest();
            else
                GL::DisableDepthTest();
            
            if(settings.cullFace)
                GL::EnableCullFace();
            else
                GL::DisableCullFace();
            
            if(settings.alphaBlend)
                GL::EnableBlendMode();
            else
                GL::DisableBlendMode();

            GL::SetDepthFunc(settings.depthFunc);

            material->Use(transform, camera);

            pMesh->GetVAO()->Bind();

            if(pMesh->GetEBO()->GetId() > 0)
                glDrawElements(GL_TRIANGLES, pMesh->GetIndicesCount(), GL_UNSIGNED_INT, 0);
            else
                glDrawArrays(GL_TRIANGLES, 0, pMesh->GetVerticesCount());

            pMesh->GetVAO()->Unbind();
        }
    }
};