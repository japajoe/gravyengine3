#ifndef GRAVYENGINE_COMPONENT_HPP
#define GRAVYENGINE_COMPONENT_HPP

#include "Object.hpp"

namespace GravyEngine
{
    class GameObject;
    class Transform;

    class Component : public Object
    {
    friend class GameObject;
    public:
        Component();
        virtual ~Component();
        GameObject *GetGameObject() const;
        Transform *GetTransform() const;
    protected:
        virtual void OnInitialize();
        virtual void OnDestroy();
        virtual void OnActivate();
        virtual void OnDeactivate();
    private:
        GameObject *gameObject;
        Transform *transform;
    };
};
#endif