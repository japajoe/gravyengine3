#include "Graphics.hpp"
#include "Renderer.hpp"
#include "MeshRenderer.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Texture2D.hpp"
#include "Shaders/DiffuseShader.hpp"
#include "../System/Drawing/Image.hpp"
#include "../Core/Camera.hpp"
#include "../Core/GameObject.hpp"
#include "../External/glad/glad.h"
#include <iostream>
#include <memory>

namespace GravyEngine
{
    static std::unique_ptr<GameObject> mainCamera;
    std::vector<Renderer*> Graphics::renderers;

    void Graphics::Initialize()
    {
        mainCamera = std::make_unique<GameObject>();
        mainCamera->AddComponent<Camera>();

        DiffuseShader::Create();

        Mesh::Add("Capsule", MeshGenerator::CreateCapsule(Vector3f::One()));
        Mesh::Add("Cube", MeshGenerator::CreateCube(Vector3f::One()));
        Mesh::Add("Plane", MeshGenerator::CreatePlane(Vector3f::One()));
        Mesh::Add("Skybox", MeshGenerator::CreateSkybox(Vector3f::One()));
        Mesh::Add("Quad", MeshGenerator::CreateQuad(Vector3f::One()));
        Mesh::Add("Sphere", MeshGenerator::CreateSphere(Vector3f::One()));
        Mesh::Add("Hemisphere", MeshGenerator::CreateHemisphere(Vector3f::One()));

        Image defaultImage(32, 32, 4, Color::White());
        if(defaultImage.IsLoaded())
        {
            Texture2D texture(&defaultImage);
            Texture2D::Add("Default", texture);
        }
    }

    void Graphics::Deinitialize()
    {
        DiffuseShader::Destroy();

        Mesh::Remove("Capsule");
        Mesh::Remove("Cube");
        Mesh::Remove("Plane");
        Mesh::Remove("Skybox");
        Mesh::Remove("Quad");
        Mesh::Remove("Sphere");
        Mesh::Remove("Hemisphere");

        Texture2D::Remove("Default");
    }

    void Graphics::OnRender()
    {
        Camera *pCamera = Camera::GetMain();

        auto color = pCamera->GetClearColor();

        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);

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
};