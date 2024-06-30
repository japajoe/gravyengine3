#ifndef GRAVYENGINE_BOUNDINGSPHERE_HPP
#define GRAVYENGINE_BOUNDINGSPHERE_HPP

#include "../System/Numerics/Vector3.hpp"
#include "../Physics/Ray.hpp"
#include "../System/Numerics/Matrix4.hpp"

namespace GravyEngine
{
    class BoundingSphere
    {
    public:
        BoundingSphere();
        void Grow(const Vector3 &point);
        float GetRadius() const;
        Vector3 GetCenter() const;
        void Clear();
        bool Intersects(const Ray &ray, float &distance) const;
    private:
        float radius;
        Vector3 center;
    };
};

#endif