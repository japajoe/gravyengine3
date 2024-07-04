#ifndef GRAVYENGINE_PLANE_HPP
#define GRAVYENGINE_PLANE_HPP

#include "../System/Numerics/Vector3.hpp"

namespace GravyEngine
{
    struct Plane
    {
        Vector3 normal;
        float distance;
        Plane() = default;
        Plane(const Vector3 &p1, const Vector3 &normal);
        float GetSignedDistanceToPlane(const Vector3& point) const;
    };
};

#endif