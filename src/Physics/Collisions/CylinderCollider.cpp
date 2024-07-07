#include "CylinderCollider.hpp"

namespace GravyEngine
{
    CylinderCollider::CylinderCollider() : Collider()
    {
        this->type = ColliderType::Cylinder;
        this->center = Vector3(0, 0, 0);
        this->size = Vector3(0.5f, 1, 0.5f);
        SetName("CylinderCollider");
    }

    void CylinderCollider::SetSize(const Vector3 &size)
    {
        this->size = size;
    }

    Vector3 CylinderCollider::GetSize() const
    {
        return size;
    }
};