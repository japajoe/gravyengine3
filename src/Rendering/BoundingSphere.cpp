#include "BoundingSphere.hpp"
#include "../System/Mathf.hpp"

namespace GravyEngine
{
    BoundingSphere::BoundingSphere()
    {
        Clear();
    }

    void BoundingSphere::Grow(const Vector3 &point)
    {
        // Calculate distance from the sphere's center to the new point
        double distToNewPoint = Vector3f::Distance(center, point);

        // If the new point is outside the current sphere, expand the sphere
        if (distToNewPoint > radius) 
        {
            // Calculate new radius
            float newRadius = (radius + distToNewPoint) * 0.5f;
            float scale = (newRadius - radius) / distToNewPoint;
            
            // Update sphere center and radius
            center.x += (point.x - center.x) * scale;
            center.y += (point.y - center.y) * scale;
            center.z += (point.z - center.z) * scale;
            radius = newRadius;
        }
    }

    float BoundingSphere::GetRadius() const
    {
        return radius;
    }

    Vector3 BoundingSphere::GetCenter() const
    {
        return center;
    }

    void BoundingSphere::Clear()
    {
        this->center = Vector3(0, 0, 0);
        this->radius = 0.0f;
    }

    bool BoundingSphere::Intersects(const Ray &ray, float &distance) const
    {
        Vector3 m = ray.origin - center;

        const float b = Vector3f::Dot(m, ray.direction);
        const float c = Vector3f::Dot(m, m) - radius * radius;

        if (c > 0.0f && b > 0.0f)
        {
            distance = 0.0f;
            return false;
        }

        const float discriminant = b * b - c;

        if (discriminant < 0.0f)
        {
            distance = 0.0f;
            return false;
        }

        distance = -b - Mathf::Sqrt(discriminant);

        if (distance < 0.0f)
            distance = 0.0f;

        return true;
    }
};