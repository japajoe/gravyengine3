#include "GameObject.hpp"
#include "../Rendering/Graphics.hpp"
#include "../Rendering/Mesh.hpp"
#include "../Rendering/MeshRenderer.hpp"
#include "../Rendering/Texture.hpp"
#include "../Rendering/Materials/DiffuseMaterial.hpp"

namespace GravyEngine
{
    GameObject::GameObject() : Object()
    {
        transform.gameObject = this;
    }

    GameObject::~GameObject()
    {
        for(size_t i = 0; i < components.size(); i++)
            components[i]->OnDestroy();
        components.clear();
    }

    Transform *GameObject::GetTransform()
    {
        return &transform;
    }

    std::shared_ptr<GameObject> GameObject::CreatePrimitive(PrimitiveType type)
    {
        Mesh *mesh = nullptr;

        switch(type)
        {
            case PrimitiveType::Cube:
                mesh = Mesh::Find("Cube");
                break;
            case PrimitiveType::Plane:
                mesh = Mesh::Find("Plane");
                break;
            case PrimitiveType::Sphere:
                mesh = Mesh::Find("Sphere");
                break;
            default:
                return nullptr;
        }

        auto object = std::make_unique<GameObject>();
        auto renderer = object->AddComponent<MeshRenderer>();
        auto material = std::make_shared<DiffuseMaterial>();
        auto texture = Texture2D::Find("Default");

        material->SetDiffuseTexture(texture);
        
        renderer->SetMaterial(material);
        renderer->SetMesh(mesh);
        
        Graphics::AddRenderer(object.get());
        
        return object;
    }
};