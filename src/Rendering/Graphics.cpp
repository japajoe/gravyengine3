#include "Graphics.hpp"
#include "Mesh.hpp"
#include "Shaders/DiffuseShader.hpp"
#include "../External/glad/glad.h"
#include <iostream>

namespace GravyEngine
{
    void Graphics::Initialize()
    {
        DiffuseShader::Create();

        Mesh::Add("Capsule", MeshGenerator::CreateCapsule(Vector3f::One()));
        Mesh::Add("Cube", MeshGenerator::CreateCube(Vector3f::One()));
        Mesh::Add("Plane", MeshGenerator::CreatePlane(Vector3f::One()));
        Mesh::Add("Skybox", MeshGenerator::CreateSkybox(Vector3f::One()));
        Mesh::Add("Quad", MeshGenerator::CreateQuad(Vector3f::One()));
        Mesh::Add("Sphere", MeshGenerator::CreateSphere(Vector3f::One()));
        Mesh::Add("Hemisphere", MeshGenerator::CreateHemisphere(Vector3f::One()));
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
    }

    void Graphics::OnRender()
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
};