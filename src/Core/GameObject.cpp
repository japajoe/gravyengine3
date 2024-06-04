#include "GameObject.hpp"

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
};