#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "Object.hpp"
#include "Component.hpp"
#include "Transform.hpp"
#include <vector>
#include <memory>

namespace GravyEngine
{
    enum class PrimitiveType
    {
        Cube,
        Plane,
        Sphere
    };

    class GameObject : public Object
    {
    private:
        Transform transform;
        std::vector<std::unique_ptr<Component>> components;
    public:
        GameObject();
        ~GameObject();
        Transform *GetTransform();

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