#include "Resources.hpp"
#include "Debug.hpp"
#include "Light.hpp"
#include "Camera.hpp"
#include "WorldSettings.hpp"
#include "../System/Drawing/Image.hpp"
#include "../Rendering/CascadedShadowMap.hpp"
#include "../Rendering/Shaders/ShaderCore.hpp"
#include "../Rendering/Shaders/BasicShader.hpp"
#include "../Rendering/Shaders/DiffuseShader.hpp"
#include "../Rendering/Shaders/ProceduralSkyboxShader.hpp"
#include "../Rendering/Shaders/DepthShader.hpp"
#include "../Rendering/Shaders/LineShader.hpp"
#include "../Rendering/Shaders/ParticleShader.hpp"
#include "../Rendering/Shaders/TerrainShader.hpp"

namespace GravyEngine
{
    std::unordered_map<std::string,Texture2D> Resources::textures2D;
    std::unordered_map<std::string,Texture2DArray> Resources::textures2DArray;
    std::unordered_map<std::string,Shader> Resources::shaders;
    std::unordered_map<std::string,UniformBufferObject> Resources::ubos;
    std::unordered_map<std::string,Mesh> Resources::meshes;

    void Resources::Initialize()
    {
        CreateTextures();
        CreateShaders();
        CreateMeshes();
        CreateUniformBuffers();
    }

    void Resources::Deinitialize()
    {
        for(auto &item : textures2D)
        {
            Debug::WriteLog("[TEXTURE] %s deleted with ID: %llu", item.first.c_str(), item.second.GetId());
            item.second.Delete();
        }

        for(auto &item : textures2DArray)
        {
            Debug::WriteLog("[TEXTURE] %s deleted with ID: %llu", item.first.c_str(), item.second.GetId());
            item.second.Delete();
        }

        for(auto &item : shaders)
        {
            Debug::WriteLog("[SHADER] %s deleted with ID: %llu", item.first.c_str(), item.second.GetId());
            item.second.Delete();
        }

        for(auto &item : ubos)
        {
            Debug::WriteLog("[UNIFORMBUFFER] %s deleted with ID: %llu", item.first.c_str(), item.second.GetId());
            item.second.Delete();
        }

        for(auto &item : meshes)
        {
            Debug::WriteLog("[MESH] %s deleted with ID: %llu", item.first.c_str(), item.second.GetInstanceId());
            item.second.Delete();
        }

        textures2D.clear();
        textures2DArray.clear();
        shaders.clear();
        ubos.clear();
        meshes.clear();
    }

    void Resources::CreateTextures()
    {
        Image defaultImage(32, 32, 4, Color::White());
        if(defaultImage.IsLoaded())
        {
            Texture2D texture(&defaultImage);
            AddTexture2D("Default", texture);
        }

        Texture2DArray depthTexture(2048, 2048, 5);
        AddTexture2DArray("DepthMap", depthTexture);
    }

    void Resources::CreateShaders()
    {
        Shader::AddIncludeFile("ShaderCore", ShaderCore::GetSource());

        AddShader("Basic", BasicShader::Create());
        AddShader("Diffuse", DiffuseShader::Create());
        AddShader("ProceduralSkybox", ProceduralSkyboxShader::Create());
        AddShader("Depth", DepthShader::Create());
        AddShader("Line", LineShader::Create());
        AddShader("Particle", ParticleShader::Create());
        AddShader("Terrain", TerrainShader::Create());
    }

    void Resources::CreateMeshes()
    {
        AddMesh("Capsule", MeshGenerator::CreateCapsule(Vector3f::One()));
        AddMesh("Cube", MeshGenerator::CreateCube(Vector3f::One()));
        AddMesh("Plane", MeshGenerator::CreatePlane(Vector3f::One()));
        AddMesh("Skybox", MeshGenerator::CreateSkybox(Vector3f::One()));
        AddMesh("Quad", MeshGenerator::CreateQuad(Vector3f::One()));
        AddMesh("Sphere", MeshGenerator::CreateSphere(Vector3f::One()));
        AddMesh("Hemisphere", MeshGenerator::CreateHemisphere(Vector3f::One()));
    }

    void Resources::CreateUniformBuffers()
    {
        AddUniformBuffer("Camera", UniformBufferObject(0, sizeof(UniformCameraInfo)));
        AddUniformBuffer("Lights", UniformBufferObject(1, Light::MaxLights * sizeof(UniformLightInfo)));
        AddUniformBuffer("Shadow", UniformBufferObject(2, sizeof(UniformShadowInfo)));
        AddUniformBuffer("World", UniformBufferObject(3, sizeof(UniformWorldInfo)));
    }

    Texture2D *Resources::AddTexture2D(const std::string &name, const Texture2D &texture)
    {
        if(textures2D.count(name) > 0)
        {
            Debug::WriteError("[TEXTURE] can't add %s with ID: %llu because it already exists", name.c_str(), texture.GetId());
            return nullptr;
        }

        textures2D[name] = texture;

        Debug::WriteLog("[TEXTURE] %s added with ID: %llu", name.c_str(), texture.GetId());

        auto pTexture = &textures2D[name];
        return pTexture;
    }

    Texture2DArray *Resources::AddTexture2DArray(const std::string &name, const Texture2DArray &texture)
    {
        if(textures2DArray.count(name) > 0)
        {
            Debug::WriteError("[TEXTURE] can't add %s with ID: %llu because it already exists", name.c_str(), texture.GetId());
            return nullptr;
        }

        textures2DArray[name] = texture;

        Debug::WriteLog("[TEXTURE] %s added with ID: %llu", name.c_str(), texture.GetId());

        auto pTexture = &textures2DArray[name];
        return pTexture;
    }

    Shader *Resources::AddShader(const std::string &name, const Shader &shader)
    {
        if(shaders.count(name) > 0)
        {
            Debug::WriteError("[SHADER] can't add %s with ID: %d because it already exists", name.c_str(), shader.GetId());
            return nullptr;
        }

        if(shader.GetId() == 0)
        {
            Debug::WriteError("[SHADER] can't add %s because it's not initialized", name.c_str());
            return nullptr;
        }

        shaders[name] = shader;

        Debug::WriteLog("[SHADER] %s created with ID: %d", name.c_str(), shader.GetId());
        
        return &shaders[name];
    }

    UniformBufferObject *Resources::AddUniformBuffer(const std::string &name, const UniformBufferObject &ubo)
    {
        if(ubos.count(name) > 0)
        {
            Debug::WriteError("[UNIFORMBUFFER] can't add %s with ID: %llu because it already exists", name.c_str(), ubo.GetId());
            return nullptr;
        }

        ubos[name] = ubo;

        Debug::WriteLog("[UNIFORMBUFFER] %s added with ID: %llu", name.c_str(), ubo.GetId());

        auto pBuffer = &ubos[name];
        return pBuffer;
    }

    Mesh *Resources::AddMesh(const std::string &name, const Mesh &mesh)
    {
        if(meshes.count(name) > 0)
        {
            Debug::WriteError("[MESH] can't add %s with ID: %llu because it already exists", name.c_str(), mesh.GetInstanceId());
            return nullptr;
        }

        meshes[name] = mesh;

        Debug::WriteLog("[MESH] %s added with ID: %llu", name.c_str(), mesh.GetInstanceId());

        auto pMesh = &meshes[name];
        pMesh->Generate();
        return pMesh;
    }

    Texture2D *Resources::FindTexture2D(const std::string &name)
    {
        if(textures2D.count(name) == 0)
            return nullptr;
        
        return &textures2D[name];
    }

    Texture2DArray *Resources::FindTexture2DArray(const std::string &name)
    {
        if(textures2DArray.count(name) == 0)
            return nullptr;
        
        return &textures2DArray[name];
    }

    Shader *Resources::FindShader(const std::string &name)
    {
        if(shaders.count(name) == 0)
            return nullptr;
        
        return &shaders[name];
    }

    UniformBufferObject *Resources::FindUniformBuffer(const std::string &name)
    {
        if(ubos.count(name) == 0)
            return nullptr;
        
        return &ubos[name];
    }

    Mesh *Resources::FindMesh(const std::string &name)
    {
        if(meshes.count(name) == 0)
            return nullptr;
        
        return &meshes[name];
    }

    void Resources::RemoveTexture2D(const std::string &name)
    {
        auto texture = FindTexture2D(name);

        if(texture)
        {
            Debug::WriteLog("[TEXTURE] %s deleted with ID: %llu", name.c_str(), texture->GetId());
            texture->Delete();
            textures2D.erase(name);
        }
    }
    
    void Resources::RemoveTexture2DArray(const std::string &name)
    {
        auto texture = FindTexture2DArray(name);

        if(texture)
        {
            Debug::WriteLog("[TEXTURE] %s deleted with ID: %llu", name.c_str(), texture->GetId());
            texture->Delete();
            textures2DArray.erase(name);
        }
    }
    
    void Resources::RemoveShader(const std::string &name)
    {
        auto shader = FindShader(name);

        if(shader)
        {
            Debug::WriteLog("[SHADER] %s deleted with ID: %d", name.c_str(), shader->GetId());
            shader->Delete();
            shaders.erase(name);
        }
    }
    
    void Resources::RemoveUniformBuffer(const std::string &name)
    {
        auto buffer = FindUniformBuffer(name);

        if(buffer)
        {
            Debug::WriteLog("[UNIFORMBUFFER] %s deleted with ID: %llu", name.c_str(), buffer->GetId());
            buffer->Delete();
            ubos.erase(name);
        }
    }

    void Resources::RemoveMesh(const std::string &name)
    {
        auto mesh = FindMesh(name);

        if(mesh)
        {
            Debug::WriteLog("[MESH] %s deleted with ID: %llu", name.c_str(), mesh->GetInstanceId());
            mesh->Delete();
            meshes.erase(name);
        }
    }
};