#include "Graphics.hpp"
#include "Renderer.hpp"
#include "MeshRenderer.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Texture2D.hpp"
#include "Shaders/ShaderCore.hpp"
#include "Shaders/DiffuseShader.hpp"
#include "Shaders/ProceduralSkyboxShader.hpp"
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
    std::vector<Renderer*> Graphics::renderers;
    std::vector<UniformBufferObject> Graphics::uniformBuffers;

    void Graphics::Initialize()
    {
        mainCamera = std::make_unique<GameObject>();
        mainCamera->AddComponent<Camera>();

        CreateTextures();
        CreateShaders();
        CreateMeshes();
        CreateUniformBuffers();

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }

    void Graphics::Deinitialize()
    {
        DestroyTextures();
        DestroyShaders();
        DestroyMeshes();
        DestroyUniformBuffers();
    }

    void Graphics::OnRender()
    {
        Camera *pCamera = Camera::GetMain();
        auto color = pCamera->GetClearColor();

        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Light::UpdateUniformBuffer();

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
            renderers.push_back(renderer);
        }
    }

    UniformBufferObject *Graphics::FindUniformBuffer(const std::string &name)
    {
        for(size_t i = 0; i < uniformBuffers.size(); i++)
        {
            if(uniformBuffers[i].GetName() == name)
            {
                return &uniformBuffers[i];
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
    }

    void Graphics::CreateShaders()
    {
        Shader::AddIncludeFile("ShaderCore", ShaderCore::GetSource());
        DiffuseShader::Create();
        ProceduralSkyboxShader::Create();
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
        CreateUniformBuffer("uLights", 3, Light::MaxLights * sizeof(UniformLightInfo));
    }

    void Graphics::DestroyTextures()
    {
        Texture2D::Remove("Default");
    }

    void Graphics::DestroyShaders()
    {
        DiffuseShader::Destroy();
        ProceduralSkyboxShader::Destroy();
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
            Debug::WriteLog("[UNIFORM_BUFFER] %s deleted with ID: %llu", buffer.GetName().c_str(), buffer.GetId());
            buffer.Delete();
        }

        uniformBuffers.clear();
    }

    UniformBufferObject *Graphics::CreateUniformBuffer(const std::string &name, uint32_t bindingIndex, size_t bufferSize)
    {
        UniformBufferObject buffer;
        buffer.SetName(name);
        buffer.Generate();
        buffer.Bind();
        buffer.BufferData(bufferSize, nullptr, GL_DYNAMIC_DRAW);
        buffer.BindBufferBase(bindingIndex);
        buffer.Unbind();
        Debug::WriteLog("[UNIFORM_BUFFER] %s added with ID: %llu", name.c_str(), buffer.GetId());
        uniformBuffers.push_back(buffer);
        return &uniformBuffers.back();
    }
};