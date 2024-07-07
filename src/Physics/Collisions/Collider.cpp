#include "Collider.hpp"

namespace GravyEngine
{
    Collider::Collider() : Component()
    {

    }
    
    Collider::~Collider()
    {

    }

    ColliderType Collider::GetType() const
    {
        return type;
    }

    void Collider::SetCenter(const Vector3 &center)
    {
        this->center = center;
    }

    Vector3 Collider::GetCenter() const
    {
        return center;
    }
};