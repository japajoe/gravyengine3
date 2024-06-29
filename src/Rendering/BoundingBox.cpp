#include "BoundingBox.hpp"
#include "../System/Mathf.hpp"

namespace GravyEngine
{
    BoundingBox::BoundingBox()
    {
        Clear();
    }

    BoundingBox::BoundingBox(const Vector3 &min, const Vector3 &max)
    {
        Clear();
        this->min = min;
        this->max = max;
        center = (min + max) * 0.5f;
        extents = max - center;
    }

    void BoundingBox::Grow(const Vector3 &point)
    {
        min = Vector3f::Min(min, point);
        max = Vector3f::Max(max, point);
        center = (min + max) * 0.5f;
        extents = max - center;
    }

    void BoundingBox::Clear()
    {
        min = Vector3f::One() * Mathf::Infinity;
        max = Vector3f::One() * -Mathf::Infinity;
        center = (min + max) * 0.5f;
        extents = max - center;
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

    Vector3 BoundingBox::GetExtents() const
    {
        return extents;
    }

    bool BoundingBox::Intersects(const Ray &ray, float &distance) const
    {
        distance = 0.0f;
        float tmax = Mathf::FloatMaxValue;

        if (Mathf::IsZero(ray.direction.x))
        {
            if (ray.origin.x < min.x || ray.origin.x > max.x)
            {
                distance = 0.0f;
                return false;
            }
        }
        else
        {
            const float inverse = 1.0f / ray.direction.x;
            float t1 = (min.x - ray.origin.x) * inverse;
            float t2 = (max.x - ray.origin.x) * inverse;

            if (t1 > t2)
            {
                const float temp = t1;
                t1 = t2;
                t2 = temp;
            }

            distance = Mathf::Max(t1, distance);
            tmax = Mathf::Min(t2, tmax);

            if (distance > tmax)
            {
                distance = 0.0f;
                return false;
            }
        }

        if (Mathf::IsZero(ray.direction.y))
        {
            if (ray.origin.y < min.y || ray.origin.y > max.y)
            {
                distance = 0.0f;
                return false;
            }
        }
        else
        {
            const float inverse = 1.0f / ray.direction.y;
            float t1 = (min.y - ray.origin.y) * inverse;
            float t2 = (max.y - ray.origin.y) * inverse;

            if (t1 > t2)
            {
                const float temp = t1;
                t1 = t2;
                t2 = temp;
            }

            distance = Mathf::Max(t1, distance);
            tmax = Mathf::Min(t2, tmax);

            if (distance > tmax)
            {
                distance = 0.0f;
                return false;
            }
        }

        if (Mathf::IsZero(ray.direction.z))
        {
            if (ray.origin.z < min.z || ray.origin.z > max.z)
            {
                distance = 0.0f;
                return false;
            }
        }
        else
        {
            const float inverse = 1.0f / ray.direction.z;
            float t1 = (min.z - ray.origin.z) * inverse;
            float t2 = (max.z - ray.origin.z) * inverse;

            if (t1 > t2)
            {
                const float temp = t1;
                t1 = t2;
                t2 = temp;
            }

            distance = Mathf::Max(t1, distance);
            tmax = Mathf::Min(t2, tmax);

            if (distance > tmax)
            {
                distance = 0.0f;
                return false;
            }
        }

        return true;
    }
};