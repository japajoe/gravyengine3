#include "Graphics.hpp"
#include "GL.hpp"
#include "LineRenderer.hpp"
#include "CascadedShadowMap.hpp"
#include "Renderer.hpp"
#include "Sprite.hpp"
#include "FullScreenQuad.hpp"
#include "Graphics2D.hpp"
#include "Time.hpp"
#include "Materials/DepthMaterial.hpp"
#include "../Audio/AudioListener.hpp"
#include "../Core/Camera.hpp"
#include "../Core/Debug.hpp"
#include "../Core/GameObject.hpp"
#include "../Core/Light.hpp"
#include "../Core/Resources.hpp"
#include "../Core/Screen.hpp"
#include "../Core/WorldSettings.hpp"
#include "../System/Mathf.hpp"
#include "../External/glad/glad.h"
#include <iostream>
#include <memory>

namespace GravyEngine
{
    static std::unique_ptr<GameObject> mainCamera;
    static std::unique_ptr<GameObject> mainLight;
    static FullScreenQuad screenQuad;
    
    std::priority_queue<Renderer*, std::vector<Renderer*>, CompareRendererOrder> Graphics::renderQueue;
    std::vector<Renderer*> Graphics::renderers;
    std::unique_ptr<Graphics2D> Graphics::graphics2D;
    std::unique_ptr<CascadedShadowMap> Graphics::cascadedShadowMap;
    std::unique_ptr<DepthMaterial> Graphics::depthMaterial;
    std::vector<FrameBufferObject> Graphics::framebuffers;

    bool CompareRendererOrder::operator()(const Renderer *lhs, const Renderer *rhs) const 
    {
        // Lower rendering order gets priority
        return lhs->GetRenderOrder() > rhs->GetRenderOrder();
    }

    void Graphics::Initialize()
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_MULTISAMPLE);

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
        Sprite::Initialize();

        auto resolution = Screen::GetResolution();

        framebuffers.push_back(FrameBufferObject(Screen::GetWidth(), Screen::GetHeight()));
        framebuffers[0].Generate();

        screenQuad.Generate();

        graphics2D = std::make_unique<Graphics2D>();
        graphics2D->Initialize();
    }

    void Graphics::Deinitialize()
    {
        Resources::Deinitialize();
        DestroyShadowMap();
        DestroyRenderers();
        LineRenderer::Deinitialize();
        Sprite::Deinitialize();
        graphics2D->Initialize();
    }

    void Graphics::OnResize(uint32_t width, uint32_t height)
    {
        glViewport(0, 0, width, height);

        auto framebuffer = GetFrameBuffer();

        if(framebuffer == nullptr)
            return;

        framebuffer->Resize(width, height);

        graphics2D->SetViewport(0, 0, width, height);
    }

    void Graphics::OnRender()
    {
        Camera::UpdateUniformBuffer();
        Light::UpdateUniformBuffer();
        WorldSettings::UpdateUniformBuffer();
        
        RenderShadowMap();
        
        framebuffers[0].Bind();

        RenderScene();

        Sprite::OnRender();

        graphics2D->NewFrame(Time::GetDeltaTime());
        
        LineRenderer::OnRender();

        framebuffers[0].Unbind();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);
        GL::DisableDepthTest();

        screenQuad.Render();
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

            for(size_t i = 0; i < renderers.size(); i++)
            {
                renderers[i]->OnRender(material, camera);
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

    FrameBufferObject *Graphics::GetFrameBuffer()
    {
        if(framebuffers.size() == 0)
            return nullptr;
        return &framebuffers[0];
    }

    Graphics2D *Graphics::GetGraphics2D() 
    {
        return graphics2D.get();
    }
};