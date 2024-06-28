#ifndef GRAVYENGINE_BOUNDINGBOX_HPP
#define GRAVYENGINE_BOUNDINGBOX_HPP

#include "../System/Numerics/Vector3.hpp"
#include "../Physics/Ray.hpp"
#include "../System/Numerics/Matrix4.hpp"

namespace GravyEngine
{
    class BoundingBox
    {
    public:
        BoundingBox();
        BoundingBox(const Vector3 &min, const Vector3 &max);
        void Grow(const Vector3 &point);
        Vector3 GetMin() const;
        Vector3 GetMax() const;
        Vector3 GetCenter() const;
        Vector3 GetExtents() const;
        void Clear();
        bool Intersects(const Ray &ray, const Matrix4 &transformation, float &distance) const;
    private:
        Vector3 min;
        Vector3 max;
        Vector3 center;
        Vector3 extents;
    };
};

#endif