#include "CapsuleCollider.hpp"

namespace GravyEngine
{
    CapsuleCollider::CapsuleCollider() : Collider()
    {
        this->type = ColliderType::Capsule;
        this->center = Vector3(0, 0, 0);
        this->radius = 0.5f;
        this->height = 1.0f;
        SetName("CapsuleCollider");
    }

    void CapsuleCollider::SetRadius(float radius)
    {
        this->radius = radius;
    }

    float CapsuleCollider::GetRadius() const
    {
        return radius;
    }

    void CapsuleCollider::SetHeight(float height)
    {
        this->height = height;
    }

    float CapsuleCollider::GetHeight() const
    {
        return height;
    }
};