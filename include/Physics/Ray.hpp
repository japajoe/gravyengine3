#ifndef GRAVYENGINE_RAY_HPP
#define GRAVYENGINE_RAY_HPP

#include "../System/Numerics/Vector2.hpp"
#include "../System/Numerics/Vector3.hpp"
#include "../System/Numerics/Vector4.hpp"

namespace GravyEngine
{
    struct Ray
    {
        Vector3 origin;
        Vector3 direction;
        float length;
        Ray();
        Ray(const Vector3 &origin, const Vector3 &direction, float length);
        static Ray FromMousePosition(const Vector2 &mousePosition, const Vector4 &viewportRect);
    };
};

#endif