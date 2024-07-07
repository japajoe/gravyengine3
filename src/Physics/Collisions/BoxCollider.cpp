#include "BoxCollider.hpp"

namespace GravyEngine
{
    BoxCollider::BoxCollider() : Collider()
    {
        this->type = ColliderType::Box;
        this->center = Vector3(0, 0, 0);
        this->size = Vector3(1, 1, 1);
        SetName("BoxCollider");
    }

    void BoxCollider::SetSize(const Vector3 &size)
    {
        this->size = size;
    }

    Vector3 BoxCollider::GetSize() const
    {
        return size;
    }
};