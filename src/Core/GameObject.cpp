#include "GameObject.hpp"
#include "Resources.hpp"
#include "../Rendering/Graphics.hpp"
#include "../Rendering/Mesh.hpp"
#include "../Rendering/MeshRenderer.hpp"
#include "../Rendering/ParticleSystem.hpp"
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
        layer = Layer::Default;
    }

    GameObject::~GameObject()
    {
        for(size_t i = 0; i < components.size(); i++)
        {
            components[i]->OnDestroy();
        }
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

    void GameObject::SetLayer(Layer layer, bool recursive)
    {
        this->layer = layer;

        if(recursive)
        {
            auto &children = transform.GetChildren();

            for(size_t i = 0; i < children.size(); i++)
            {
                children[i]->GetGameObject()->SetLayer(layer);
            }
        }
    }

    Layer GameObject::GetLayer() const
    {
        return layer;
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
        uint32_t renderOrder = 1000;
        std::string name = "GameObject";

        switch(type)
        {
            case PrimitiveType::Capsule:
            {
                mesh = Resources::FindMesh("Capsule");
                createMaterial = [] () { return std::make_shared<DiffuseMaterial>(); };
                pTexture = Resources::FindTexture2D("Default");
                name = "Capsule";
                break;
            }
            case PrimitiveType::Cube:
            {
                mesh = Resources::FindMesh("Cube");
                createMaterial = [] () { return std::make_shared<DiffuseMaterial>(); };
                pTexture = Resources::FindTexture2D("Default");
                name = "Cube";
                break;
            }
            case PrimitiveType::Hemisphere:
            {
                mesh = Resources::FindMesh("Hemisphere");
                createMaterial = [] () { return std::make_shared<DiffuseMaterial>(); };
                pTexture = Resources::FindTexture2D("Default");
                name = "Hemisphere";
                break;
            }
            case PrimitiveType::Plane:
            {
                mesh = Resources::FindMesh("Plane");
                createMaterial = [] () { return std::make_shared<DiffuseMaterial>(); };
                pTexture = Resources::FindTexture2D("Default");
                name = "Plane";
                break;
            }
            case PrimitiveType::Quad:
            {
                mesh = Resources::FindMesh("Quad");
                createMaterial = [] () { return std::make_shared<DiffuseMaterial>(); };
                pTexture = Resources::FindTexture2D("Default");
                name = "Quad";
                break;
            }
            case PrimitiveType::ParticleSystem:
            {
                GameObject *obj = GameObject::Create();
                obj->SetName("ParticleSystem");
                auto ps = obj->AddComponent<ParticleSystem>();
                ps->SetRenderOrder(1001);
                return obj;
            }
            case PrimitiveType::Skybox:
            {
                mesh = Resources::FindMesh("Hemisphere");
                createMaterial = [] () { return std::make_shared<ProceduralSkyboxMaterial>(); };
                cullFace = false;
                depthTest = false;
                castShadows = false;
                name = "Skybox";
                renderOrder = 999;
                break;
            }
            case PrimitiveType::Sphere:
            {
                mesh = Resources::FindMesh("Sphere");
                createMaterial = [] () { return std::make_shared<DiffuseMaterial>(); };
                pTexture = Resources::FindTexture2D("Default");
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
        renderer->SetRenderOrder(renderOrder);
        
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

    void GameObject::DestroyAll()
    {
        objects.clear();
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