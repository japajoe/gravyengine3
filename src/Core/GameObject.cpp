#include "GameObject.hpp"
#include "../Rendering/Graphics.hpp"
#include "../Rendering/Mesh.hpp"
#include "../Rendering/MeshRenderer.hpp"
#include "../Rendering/Texture.hpp"
#include "../Rendering/Materials/DiffuseMaterial.hpp"
#include "../Rendering/Materials/ProceduralSkyboxMaterial.hpp"

namespace GravyEngine
{
    std::vector<std::unique_ptr<GameObject>> GameObject::objects;
    std::vector<GameObject*> GameObject::destroyQueue;

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

        auto &children = transform.GetChildren();

        for(size_t i = 0; i < children.size(); i++)
        {
            children[i]->GetGameObject()->SetIsActive(isActive);
        }
    }

    bool GameObject::GetIsActive() const
    {
        return isActive;
    }

    GameObject *GameObject::Create()
    {
        objects.push_back(std::make_unique<GameObject>());
        return objects.back().get();
    }

    GameObject *GameObject::CreatePrimitive(PrimitiveType type)
    {
        Mesh *mesh = nullptr;

        std::function<std::shared_ptr<Material>()> createMaterial;
        Texture2D *pTexture = nullptr;

        bool cullFace = true;
        bool depthTest = true;
        bool castShadows = true;
        std::string name = "GameObject";

        switch(type)
        {
            case PrimitiveType::Capsule:
            {
                mesh = Mesh::Find("Capsule");
                createMaterial = [] () { return std::make_shared<DiffuseMaterial>(); };
                pTexture = Texture2D::Find("Default");
                name = "Capsule";
                break;
            }
            case PrimitiveType::Cube:
            {
                mesh = Mesh::Find("Cube");
                createMaterial = [] () { return std::make_shared<DiffuseMaterial>(); };
                pTexture = Texture2D::Find("Default");
                name = "Cube";
                break;
            }
            case PrimitiveType::Hemisphere:
            {
                mesh = Mesh::Find("Hemisphere");
                createMaterial = [] () { return std::make_shared<DiffuseMaterial>(); };
                pTexture = Texture2D::Find("Default");
                name = "Hemisphere";
                break;
            }
            case PrimitiveType::Plane:
            {
                mesh = Mesh::Find("Plane");
                createMaterial = [] () { return std::make_shared<DiffuseMaterial>(); };
                pTexture = Texture2D::Find("Default");
                name = "Plane";
                break;
            }
            case PrimitiveType::Quad:
            {
                mesh = Mesh::Find("Quad");
                createMaterial = [] () { return std::make_shared<DiffuseMaterial>(); };
                pTexture = Texture2D::Find("Default");
                name = "Quad";
                break;
            }
            case PrimitiveType::Skybox:
            {
                mesh = Mesh::Find("Hemisphere");
                createMaterial = [] () { return std::make_shared<ProceduralSkyboxMaterial>(); };
                cullFace = false;
                depthTest = false;
                castShadows = false;
                name = "Skybox";
                break;
            }
            case PrimitiveType::Sphere:
            {
                mesh = Mesh::Find("Sphere");
                createMaterial = [] () { return std::make_shared<DiffuseMaterial>(); };
                pTexture = Texture2D::Find("Default");
                name = "Sphere";
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

        auto object = GameObject::Create();
        object->SetName(name);
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
        
        return object;
    }

    void GameObject::Destroy(GameObject *object)
    {
        if(!object)
            return;

        destroyQueue.push_back(object);
    }

    void GameObject::DestroyImmediate(GameObject *object)
    {
        for(size_t i = 0; i < object->components.size(); i++)
        {
            object->components[i]->OnDestroy();
        }

        auto children = object->GetTransform()->GetChildrenRecursive();

        for(size_t i = 0; i < children.size(); i++)
        {
            GameObject *currentObject = children[i]->GetGameObject();
            for(size_t i = 0; i < currentObject->components.size(); i++)
            {
                currentObject->components[i]->OnDestroy();
            }
        }

        for(size_t i = 0; i < children.size(); i++)
        {
            RemoveObject(children[i]->GetGameObject());
        }

        RemoveObject(object);
    }

    void GameObject::RemoveObject(GameObject *object)
    {
        if(!object)
            return;
        size_t index = 0;
        bool found = false;

        for(size_t i = 0; i < objects.size(); i++)
        {
            if(object->GetInstanceId() == objects[i]->GetInstanceId())
            {
                index = i;
                found = true;
                break;
            }
        }

        if(found)
        {
            object->GetTransform()->SetParent(nullptr);
            objects.erase(objects.begin() + index);
        }
    }

    void GameObject::OnEndFrame()
    {
        if(destroyQueue.size() == 0)
            return;

        for(size_t i = 0; i < destroyQueue.size(); i++)
        {
            GameObject *object = destroyQueue[i];

            if(!object)
                continue;

            DestroyImmediate(object);
        }

        destroyQueue.clear();
    }
};