#ifndef GRAVYENGINE_GAMEOBJECT_HPP
#define GRAVYENGINE_GAMEOBJECT_HPP

#include "Object.hpp"
#include "Component.hpp"
#include "Transform.hpp"
#include <vector>
#include <memory>

namespace GravyEngine
{
    enum class PrimitiveType
    {
        Capsule,
        Cube,
        Hemisphere,
        Plane,
        Quad,
        ParticleSystem,
        Skybox,
        Sphere,
        Terrain
    };

    enum class Layer : uint32_t
    {
        Default = 1 << 0,
        IgnoreRaycast = 1 << 1
    };

    class GameObject : public Object
    {
    friend class GameBehaviourManager;
    private:
        Transform transform;
        bool isActive;
        Layer layer;
        std::vector<std::unique_ptr<Component>> components;
        static std::vector<std::unique_ptr<GameObject>> objects;
        static std::vector<GameObject*> destroyQueue;
        static void DestroyImmediate(GameObject *object);
        static void OnEndFrame();
        static void RemoveObject(GameObject *object);
        static void DestroyAll();
    public:
        GameObject();
        ~GameObject();
        Transform *GetTransform();
        void SetIsActive(bool isActive);
        bool GetIsActive() const;
        void SetLayer(Layer layer, bool recursive = true);
        Layer GetLayer() const;
        static void Destroy(GameObject *object);        
        static GameObject *Create();
        static GameObject *CreatePrimitive(PrimitiveType type);

        template <typename T>
        T *GetComponent() const
        {
            static_assert(std::is_base_of<Component, T>::value, "GetComponent parameter must derive from Component");

            T *ptr = nullptr;

            for(size_t i = 0; i < components.size(); i++)
            {
                ptr = dynamic_cast<T*>(components[i].get());
                if(ptr)
                    return ptr;
            }
            return nullptr;
        }

        template <typename T, typename... Param>
        T* AddComponent(Param... param)
        {
            static_assert(std::is_base_of<Component, T>::value, "AddComponent parameter must derive from Component");

            std::unique_ptr<Component> ptr = std::make_unique<T>(param...);
            if (!ptr)
                return nullptr;

            components.push_back(std::move(ptr));
            size_t last = components.size() - 1;
            Component *component = components[last].get();
            component->gameObject = this;
            component->transform = &this->transform;
            component->OnInitialize();

            return static_cast<T*>(component);
        }
    };
};

#endif