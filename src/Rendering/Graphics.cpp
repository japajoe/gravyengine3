#include "Graphics.hpp"
#include "LineRenderer.hpp"
#include "CascadedShadowMap.hpp"
#include "Renderer.hpp"
#include "MeshRenderer.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Texture2D.hpp"
#include "Texture2DArray.hpp"
#include "Shaders/ShaderCore.hpp"
#include "Shaders/UniformLight.hpp"
#include "Shaders/UniformCamera.hpp"
#include "Shaders/UniformShadow.hpp"
#include "Shaders/DiffuseShader.hpp"
#include "Shaders/ProceduralSkyboxShader.hpp"
#include "Shaders/DepthShader.hpp"
#include "Shaders/LineShader.hpp"
#include "Materials/DepthMaterial.hpp"
#include "../System/Mathf.hpp"
#include "../System/Drawing/Image.hpp"
#include "../Core/Camera.hpp"
#include "../Core/Light.hpp"
#include "../Core/GameObject.hpp"
#include "../Core/Debug.hpp"
#include "../External/glad/glad.h"
#include <iostream>
#include <memory>

namespace GravyEngine
{
    static std::unique_ptr<GameObject> mainCamera;
    static std::unique_ptr<GameObject> mainLight;
    std::vector<Renderer*> Graphics::renderers;
    std::vector<std::unique_ptr<UniformBufferObject>> Graphics::uniformBuffers;
    std::unique_ptr<CascadedShadowMap> Graphics::cascadedShadowMap;
    std::unique_ptr<DepthMaterial> Graphics::depthMaterial;

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

        LineRenderer::Deinitialize();
    }

    void Graphics::OnRender()
    {
        Camera::UpdateUniformBuffer();
        Light::UpdateUniformBuffer();
        
        RenderShadowMap();
        RenderScene();
        
        LineRenderer::OnRender();
    }

    void Graphics::RenderShadowMap()
    {
        if(renderers.size() > 0)
        {
            Camera *camera = Camera::GetMain();
            DepthMaterial *material = depthMaterial.get();

            cascadedShadowMap->Bind();
            for(size_t i = 0; i < renderers.size(); i++)
            {
                if(!renderers[i]->GetCastShadows())
                    continue;
                renderers[i]->OnRender(material, camera);
            }
            cascadedShadowMap->Unbind();
        }
    }

    void Graphics::RenderScene()
    {
        Camera *pCamera = Camera::GetMain();
        auto color = pCamera->GetClearColor();
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(renderers.size() > 0)
        {
            for(size_t i = 0; i < renderers.size(); i++)
            {
                renderers[i]->OnRender();
            }
        }
    }

    void Graphics::AddRenderer(GameObject *obj)
    {
        Renderer *renderer = obj->GetComponent<Renderer>();

        if(renderer)
        {
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
        }
    }

    void Graphics::RemoveRenderer(GameObject *obj)
    {
        Renderer *renderer = obj->GetComponent<Renderer>();

        if(renderer)
        {
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
            }
        }
    }

    UniformBufferObject *Graphics::FindUniformBuffer(const std::string &name)
    {
        for(size_t i = 0; i < uniformBuffers.size(); i++)
        {
            if(uniformBuffers[i]->GetName() == name)
            {
                return uniformBuffers[i].get();
            }
        }

        return nullptr;
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
        Shader::AddIncludeFile("UniformLight", UniformLight::GetSource());
        Shader::AddIncludeFile("UniformCamera", UniformCamera::GetSource());
        Shader::AddIncludeFile("UniformShadow", UniformShadow::GetSource());

        DiffuseShader::Create();
        ProceduralSkyboxShader::Create();
        DepthShader::Create();
        LineShader::Create();
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
        auto diffuseShader = Shader::Find("Diffuse");

        if(diffuseShader == nullptr)
        {
            Debug::WriteError("Can't create uniform buffer because Diffuse shader is null");
            return;
        }

        CreateUniformBuffer("Camera", 0, sizeof(UniformCameraInfo), {diffuseShader});
        CreateUniformBuffer("Lights", 1, Light::MaxLights * sizeof(UniformLightInfo), {diffuseShader});
        CreateUniformBuffer("Shadow", 2, sizeof(UniformShadowInfo), {diffuseShader});
    }

    void Graphics::CreateShadowMap()
    {
        Texture2DArray *depthMap = Texture2DArray::Find("DepthMap");
        UniformBufferObject *shadowData = FindUniformBuffer("Shadow");
        Camera *camera = mainCamera->GetComponent<Camera>();
        Light *light = mainLight->GetComponent<Light>();

        cascadedShadowMap = std::make_unique<CascadedShadowMap>(depthMap, shadowData, camera, light);
        
        depthMaterial = std::make_unique<DepthMaterial>();
    }

    void Graphics::DestroyTextures()
    {
        Texture2D::Remove("Default");
        Texture2DArray::Remove("DepthMap");
    }

    void Graphics::DestroyShaders()
    {
        DiffuseShader::Destroy();
        ProceduralSkyboxShader::Destroy();
        DepthShader::Destroy();
        LineShader::Destroy();
    }

    void Graphics::DestroyMeshes()
    {
        Mesh::Remove("Capsule");
        Mesh::Remove("Cube");
        Mesh::Remove("Plane");
        Mesh::Remove("Skybox");
        Mesh::Remove("Quad");
        Mesh::Remove("Sphere");
        Mesh::Remove("Hemisphere");
    }

    void Graphics::DestroyUniformBuffers()
    {
        for(size_t i = 0; i < uniformBuffers.size(); i++)
        {
            auto &buffer = uniformBuffers[i];
            Debug::WriteLog("[UNIFORM_BUFFER] %s deleted with ID: %llu", buffer->GetName().c_str(), buffer->GetId());
            buffer->Delete();
        }

        uniformBuffers.clear();
    }

    void Graphics::DestroyShadowMap()
    {
        cascadedShadowMap->Delete();
    }

    UniformBufferObject *Graphics::CreateUniformBuffer(const std::string &name, uint32_t bindingIndex, size_t bufferSize, const std::vector<Shader*> &shaders)
    {
        uniformBuffers.push_back(std::make_unique<UniformBufferObject>());
        size_t last = uniformBuffers.size() -1;
        auto pBuffer = uniformBuffers[last].get();

        // for(size_t i = 0; i < shaders.size(); i++)
        //     pBuffer->BindToShader(shaders[i]->GetId(), bindingIndex, name);

        pBuffer->SetName(name);
        pBuffer->Generate();
        pBuffer->Bind();
        pBuffer->BufferData(bufferSize, nullptr, GL_DYNAMIC_DRAW);
        pBuffer->BindBufferBase(bindingIndex);
        pBuffer->Unbind();
        Debug::WriteLog("[UNIFORM_BUFFER] %s added with ID: %llu", name.c_str(), pBuffer->GetId());
        return pBuffer;
    }
};