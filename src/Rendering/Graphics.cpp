#include "Graphics.hpp"
#include "LineRenderer.hpp"
#include "CascadedShadowMap.hpp"
#include "Renderer.hpp"
#include "MeshRenderer.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Texture2D.hpp"
#include "Texture2DArray.hpp"
#include "Buffers/UniformBufferObject.hpp"
#include "Materials/DepthMaterial.hpp"
#include "Shaders/ShaderCore.hpp"
#include "Shaders/DiffuseShader.hpp"
#include "Shaders/ProceduralSkyboxShader.hpp"
#include "Shaders/DepthShader.hpp"
#include "Shaders/LineShader.hpp"
#include "Shaders/ParticleShader.hpp"
#include "Shaders/TerrainShader.hpp"
#include "../System/Mathf.hpp"
#include "../System/Drawing/Image.hpp"
#include "../Core/Camera.hpp"
#include "../Core/Light.hpp"
#include "../Core/GameObject.hpp"
#include "../Core/Debug.hpp"
#include "../Core/WorldSettings.hpp"
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

        mainLight = std::make_unique<GameObject>();
        auto light = mainLight->AddComponent<Light>();
        light->SetType(LightType::Directional);
        light->GetTransform()->SetPosition(Vector3(0, 10, 0));
        light->GetTransform()->SetRotation(Quaternionf::Euler(45 * Mathf::Deg2Rad, 0, 0));

        CreateTextures();
        CreateShaders();
        CreateMeshes();
        CreateUniformBuffers();
        CreateShadowMap();

        LineRenderer::Initialize();

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }

    void Graphics::Deinitialize()
    {
        DestroyTextures();
        DestroyShaders();
        DestroyMeshes();
        DestroyUniformBuffers();
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
        Camera *camera = Camera::GetMain();

        if(renderQueue.size() > 0)
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
        auto color = camera->GetClearColor();
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(renderQueue.size() > 0)
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

    void Graphics::CreateTextures()
    {
        Image defaultImage(32, 32, 4, Color::White());
        if(defaultImage.IsLoaded())
        {
            Texture2D texture(&defaultImage);
            Texture2D::Add("Default", texture);
        }

        Texture2DArray depthTexture(2048, 2048, 5);
        Texture2DArray::Add("DepthMap", depthTexture);
    }

    void Graphics::CreateShaders()
    {
        Shader::AddIncludeFile("ShaderCore", ShaderCore::GetSource());

        DiffuseShader::Create();
        ProceduralSkyboxShader::Create();
        DepthShader::Create();
        LineShader::Create();
        ParticleShader::Create();
        TerrainShader::Create();
    }

    void Graphics::CreateMeshes()
    {
        Mesh::Add("Capsule", MeshGenerator::CreateCapsule(Vector3f::One()));
        Mesh::Add("Cube", MeshGenerator::CreateCube(Vector3f::One()));
        Mesh::Add("Plane", MeshGenerator::CreatePlane(Vector3f::One()));
        Mesh::Add("Skybox", MeshGenerator::CreateSkybox(Vector3f::One()));
        Mesh::Add("Quad", MeshGenerator::CreateQuad(Vector3f::One()));
        Mesh::Add("Sphere", MeshGenerator::CreateSphere(Vector3f::One()));
        Mesh::Add("Hemisphere", MeshGenerator::CreateHemisphere(Vector3f::One()));
    }

    void Graphics::CreateUniformBuffers()
    {
        UniformBufferObject::Add("Camera", UniformBufferObject(0, sizeof(UniformCameraInfo)));
        UniformBufferObject::Add("Lights", UniformBufferObject(1, Light::MaxLights * sizeof(UniformLightInfo)));
        UniformBufferObject::Add("Shadow", UniformBufferObject(2, sizeof(UniformShadowInfo)));
        UniformBufferObject::Add("World", UniformBufferObject(3, sizeof(UniformWorldInfo)));
    }

    void Graphics::CreateShadowMap()
    {
        Texture2DArray *depthMap = Texture2DArray::Find("DepthMap");
        UniformBufferObject *shadowData = UniformBufferObject::Find("Shadow");
        Camera *camera = mainCamera->GetComponent<Camera>();
        Light *light = mainLight->GetComponent<Light>();

        cascadedShadowMap = std::make_unique<CascadedShadowMap>(depthMap, shadowData, camera, light);
        
        depthMaterial = std::make_unique<DepthMaterial>();
    }

    void Graphics::DestroyTextures()
    {
        Texture2D::RemoveAll();
        Texture2DArray::RemoveAll();
    }

    void Graphics::DestroyShaders()
    {
        Shader::RemoveAll();
    }

    void Graphics::DestroyMeshes()
    {
        Mesh::RemoveAll();
    }

    void Graphics::DestroyUniformBuffers()
    {
        UniformBufferObject::RemoveAll();
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