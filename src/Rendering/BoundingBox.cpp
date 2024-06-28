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

    bool BoundingBox::Intersects(const Ray &ray, const Matrix4 &transformation, float &distance) const
    {
        float nearestFarIntersection = 0.0f;
        float farthestNearIntersection = Mathf::FloatMaxValue;
        Vector3 worldPosition = transformation * Vector4(center.x, center.y, center.z, 1.0f);

        Vector3 bbMin = transformation * Vector4(min.x, min.y, min.z, 1.0f);
        Vector3 bbMax = transformation * Vector4(max.x, max.y, max.z, 1.0f);

        Vector3 delta = ray.origin - worldPosition;

        Vector3 xAxis = glm::vec3(transformation[0]);
        Vector3 yAxis = glm::vec3(transformation[1]);
        Vector3 zAxis = glm::vec3(transformation[2]);
        
        Vector3 axes[3] = {
            xAxis,
            yAxis,
            zAxis
        };

        for(size_t i = 0; i < 3; i++)
        {
            float e = Vector3f::Dot(axes[i], delta);
            float f = Vector3f::Dot(ray.direction, axes[i]);

            if(Mathf::Abs(f) > Mathf::Epsilon)
            {
                //Intersection with left plane
                float t1 = (e + bbMin[i]) / f;

                //Intersection with right plane
                float t2 = (e + bbMax[i]) / f;

                if(t1 > t2)
                {
                    float temp = t1;
                    t1 = t2;
                    t2 = temp;
                }

                if(t2 < farthestNearIntersection)
                    farthestNearIntersection = t2;

                if(t1 < nearestFarIntersection)
                    nearestFarIntersection = t1;

                if(farthestNearIntersection < nearestFarIntersection)
                    return false;
            }
            else
            {
                if(-e + bbMin[i] > 0.0f || -e + bbMax[i] < 0.0f)
                    return false;
            }
        }

        if(nearestFarIntersection == 0.0f)
            return false;
        
        distance = nearestFarIntersection;
        return true;
    }
};