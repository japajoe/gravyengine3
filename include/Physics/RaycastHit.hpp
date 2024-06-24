#ifndef GRAVYENGINE_RAYCASTHIT_HPP
#define GRAVYENGINE_RAYCASTHIT_HPP

#include <cstdint>
#include "../System/Numerics/Vector3.hpp"
#include "../Core/Transform.hpp"

namespace GravyEngine
{
    struct RaycastHit
    {
        float distance;
        Vector3 point;
        Vector3 normal;
        Transform *transform;
        uint32_t triangleIndex1;
        uint32_t triangleIndex2;
        uint32_t triangleIndex3;
    };
};

#endif