#include "BoundingBox.hpp"
#include "../System/Mathf.hpp"

namespace GravyEngine
{
    BoundingBox::BoundingBox()
    {
        Clear();
    }

    void BoundingBox::Grow(const Vector3 &point)
    {
        min = Vector3f::Min(min, point);
        max = Vector3f::Max(max, point);
        center = (min + max) * 0.5f;
    }

    void BoundingBox::Clear()
    {
        min = Vector3f::One() * Mathf::Infinity;
        max = Vector3f::One() * -Mathf::Infinity;
        center = (min + max) * 0.5f;
    }

    Vector3 BoundingBox::GetMin() const
    {
        return min;
    }

    Vector3 BoundingBox::GetMax() const
    {
        return max;
    }

    Vector3 BoundingBox::GetCenter() const
    {
        return center;
    }
};