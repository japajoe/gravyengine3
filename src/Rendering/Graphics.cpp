#include "Graphics.hpp"
#include "LineRenderer.hpp"
#include "CascadedShadowMap.hpp"
#include "Renderer.hpp"
#include "Materials/DepthMaterial.hpp"
#include "../Audio/AudioListener.hpp"
#include "../Core/Resources.hpp"
#include "../Core/Camera.hpp"
#include "../Core/Light.hpp"
#include "../Core/GameObject.hpp"
#include "../Core/Debug.hpp"
#include "../Core/WorldSettings.hpp"
#include "../System/Mathf.hpp"
#include "../External/glad/glad.h"
#include <iostream>
#include <memory>

namespace GravyEngine
{
    static std::unique_ptr<GameObject> mainCamera;
    static std::unique_ptr<GameObject> mainLight;
    
    std::priority_queue<Renderer*, std::vector<Renderer*>, CompareRendererOrder> Graphics::renderQueue;
    std::vector<Renderer*> Graphics::renderers;
    std::unique_ptr<CascadedShadowMap> Graphics::cascadedShadowMap;
    std::unique_ptr<DepthMaterial> Graphics::depthMaterial;

    bool CompareRendererOrder::operator()(const Renderer *lhs, const Renderer *rhs) const 
    {
        // Lower rendering order gets priority
        return lhs->GetRenderOrder() > rhs->GetRenderOrder();
    }

    void Graphics::Initialize()
    {
        mainCamera = std::make_unique<GameObject>();
        mainCamera->AddComponent<Camera>();
        mainCamera->AddComponent<AudioListener>();

        mainLight = std::make_unique<GameObject>();
        auto light = mainLight->AddComponent<Light>();
        light->SetType(LightType::Directional);
        light->GetTransform()->SetPosition(Vector3(0, 10, 0));
        light->GetTransform()->SetRotation(Quaternionf::Euler(45 * Mathf::Deg2Rad, 0, 0));

        Resources::Initialize();
        CreateShadowMap();

        LineRenderer::Initialize();

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }

    void Graphics::Deinitialize()
    {
        Resources::Deinitialize();
        DestroyShadowMap();
        DestroyRenderers();
        LineRenderer::Deinitialize();
    }

    void Graphics::OnRender()
    {
        Camera::UpdateUniformBuffer();
        Light::UpdateUniformBuffer();
        WorldSettings::UpdateUniformBuffer();
        
        RenderShadowMap();
        RenderScene();
        
        LineRenderer::OnRender();
    }

    void Graphics::RenderShadowMap()
    {
        if(!WorldSettings::GetShadowsEnabled())
            return;

        Camera *camera = Camera::GetMain();

        if(renderQueue.size() > 0 && camera)
        {
            DepthMaterial *material = depthMaterial.get();

            cascadedShadowMap->Bind();

            std::priority_queue<Renderer*, std::vector<Renderer*>, CompareRendererOrder> queue = renderQueue;
            while (!queue.empty()) 
            {
                Renderer* currentRenderer = queue.top();
                if(currentRenderer->GetCastShadows())
                    currentRenderer->OnRender(material, camera);
                queue.pop();
            }

            cascadedShadowMap->Unbind();
        }
    }

    void Graphics::RenderScene()
    {
        Camera *camera = Camera::GetMain();

        if(camera)
        {
            auto color = camera->GetClearColor();
            glClearColor(color.r, color.g, color.b, color.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        else
        {
            glClearColor(1, 1, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        if(renderQueue.size() > 0 && camera)
        {
            std::priority_queue<Renderer*, std::vector<Renderer*>, CompareRendererOrder> queue = renderQueue;

            while (!queue.empty()) 
            {
                Renderer* currentRenderer = queue.top();
                currentRenderer->OnRender();
                queue.pop();
            }
        }
    }

    void Graphics::AddRenderer(Renderer *renderer)
    {
        if(!renderer)
            return;

        for(size_t i = 0; i < renderers.size(); i++)
        {
            if(renderers[i] == renderer)
            {
                Debug::WriteError("[RENDERER] can't add with ID: %llu because it already exists", renderer->GetInstanceId());
                return;
            }
        }

        Debug::WriteLog("[RENDERER] added with ID: %llu", renderer->GetInstanceId());

        renderers.push_back(renderer);

        renderQueue.push(renderer);
    }

    void Graphics::RemoveRenderer(Renderer *renderer)
    {
        if(!renderer)
            return;

        size_t index = 0;
        bool found = false;

        for(size_t i = 0; i < renderers.size(); i++)
        {
            if(renderers[i] == renderer)
            {
                index = i;
                found = true;
                break;
            }
        }

        if(found)
        {
            Debug::WriteLog("[RENDERER] removed with ID: %llu", renderer->GetInstanceId());
            renderers.erase(renderers.begin() + index);

            //Clear render queue and recreate
            while(!renderQueue.empty())
            {
                renderQueue.pop();
            }

            for(size_t i = 0; i < renderers.size(); i++)
            {
                renderQueue.push(renderers[i]);
            }
        }
    }

    Renderer *Graphics::GetRendererByIndex(size_t index)
    {
        if(index >= renderers.size())
            return nullptr;
        return renderers[index];
    }

    void Graphics::CreateShadowMap()
    {
        Texture2DArray *depthMap = Resources::FindTexture2DArray("DepthMap");
        UniformBufferObject *shadowData = Resources::FindUniformBuffer("Shadow");
        Camera *camera = mainCamera->GetComponent<Camera>();
        Light *light = mainLight->GetComponent<Light>();

        cascadedShadowMap = std::make_unique<CascadedShadowMap>(depthMap, shadowData, camera, light);
        
        depthMaterial = std::make_unique<DepthMaterial>();
    }

    void Graphics::DestroyShadowMap()
    {
        cascadedShadowMap->Delete();
    }

    void Graphics::DestroyRenderers()
    {
        renderers.clear();

        while(!renderQueue.empty())
        {
            renderQueue.pop();
        }
    }
};