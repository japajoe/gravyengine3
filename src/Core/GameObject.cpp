#include "GameObject.hpp"
#include "../Rendering/Graphics.hpp"
#include "../Rendering/Mesh.hpp"
#include "../Rendering/MeshRenderer.hpp"
#include "../Rendering/Texture.hpp"
#include "../Rendering/Materials/DiffuseMaterial.hpp"
#include "../Rendering/Materials/ProceduralSkyboxMaterial.hpp"

namespace GravyEngine
{
    GameObject::GameObject() : Object()
    {
        transform.gameObject = this;
        isActive = true;
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

    void GameObject::SetIsActive(bool isActive)
    {
        this->isActive = isActive;
    }

    bool GameObject::GetIsActive() const
    {
        return isActive;
    }

    std::shared_ptr<GameObject> GameObject::CreatePrimitive(PrimitiveType type)
    {
        Mesh *mesh = nullptr;

        std::function<std::shared_ptr<Material>()> createMaterial;
        Texture2D *pTexture = nullptr;

        bool cullFace = true;
        bool depthTest = true;
        bool castShadows = true;

        switch(type)
        {
            case PrimitiveType::Capsule:
            {
                mesh = Mesh::Find("Capsule");
                createMaterial = [] () { return std::make_shared<DiffuseMaterial>(); };
                pTexture = Texture2D::Find("Default");
                break;
            }
            case PrimitiveType::Cube:
            {
                mesh = Mesh::Find("Cube");
                createMaterial = [] () { return std::make_shared<DiffuseMaterial>(); };
                pTexture = Texture2D::Find("Default");
                break;
            }
            case PrimitiveType::Hemisphere:
            {
                mesh = Mesh::Find("Hemisphere");
                createMaterial = [] () { return std::make_shared<DiffuseMaterial>(); };
                pTexture = Texture2D::Find("Default");
                break;
            }
            case PrimitiveType::Plane:
            {
                mesh = Mesh::Find("Plane");
                createMaterial = [] () { return std::make_shared<DiffuseMaterial>(); };
                pTexture = Texture2D::Find("Default");
                break;
            }
            case PrimitiveType::Quad:
            {
                mesh = Mesh::Find("Quad");
                createMaterial = [] () { return std::make_shared<DiffuseMaterial>(); };
                pTexture = Texture2D::Find("Default");
                break;
            }
            case PrimitiveType::Skybox:
            {
                mesh = Mesh::Find("Hemisphere");
                createMaterial = [] () { return std::make_shared<ProceduralSkyboxMaterial>(); };
                cullFace = false;
                depthTest = false;
                castShadows = false;
                break;
            }
            case PrimitiveType::Sphere:
            {
                mesh = Mesh::Find("Sphere");
                createMaterial = [] () { return std::make_shared<DiffuseMaterial>(); };
                pTexture = Texture2D::Find("Default");
                break;
            }
            default:
            {
                mesh = nullptr;
                break;
            }
        }

        if(mesh == nullptr)
            return nullptr;

        auto object = std::make_shared<GameObject>();
        auto renderer = object->AddComponent<MeshRenderer>();
        auto material = createMaterial();

        if(pTexture)
        {
            DiffuseMaterial *pMaterial = static_cast<DiffuseMaterial*>(material.get());
            pMaterial->SetDiffuseTexture(pTexture);
        }

        renderer->Add(mesh, material);
        renderer->SetCastShadows(castShadows);
        
        auto settings = renderer->GetSettings(0);
        settings->cullFace = cullFace;
        settings->depthTest = depthTest;

        Graphics::AddRenderer(object.get());
        
        return object;
    }
};