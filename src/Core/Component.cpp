#include "Component.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"

namespace GravyEngine
{
    Component::Component() : Object()
    {
        gameObject = nullptr;
        transform = nullptr;
    }

    Component::~Component()
    {
        
    }

    void Component::OnInitialize()
    {

    }

    void Component::OnDestroy()
    {

    }

    GameObject *Component::GetGameObject() const
    {
        return gameObject;
    }

    Transform *Component::GetTransform() const
    {
        return transform;
    }
};