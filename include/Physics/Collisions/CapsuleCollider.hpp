#ifndef GRAVYENGINE_CAPSULECOLLIDER_HPP
#define GRAVYENGINE_CAPSULECOLLIDER_HPP

#include "Collider.hpp"

namespace GravyEngine
{
    class CapsuleCollider : public Collider
    {
    public:
        CapsuleCollider();
        void SetRadius(float radius);
        float GetRadius() const;
        void SetHeight(float radius);
        float GetHeight() const;
    protected:
        float radius;
        float height;
    };
};

#endif