#ifndef GRAVYENGINE_PHYSICS_HPP
#define GRAVYENGINE_PHYSICS_HPP

#include "Ray.hpp"
#include "RaycastHit.hpp"

namespace GravyEngine
{
    class Physics
    {
    public:
        static bool Raycast(const Ray &ray, RaycastHit &hit, uint32_t layerMask = 0);
        static bool Raycast(const Vector3 &origin, const Vector3 &direction, float maxDistance, RaycastHit &hit, uint32_t layerMask = 0);
        static bool BoxTest(const Vector3 &origin, const Vector3 &direction, float maxDistance, RaycastHit &hit, uint32_t layerMask = 0);
    private:
        static bool LineIntersects(const Vector3 &l1p1, const Vector3 &l1p2, const Vector3 &l2p1, const Vector3 &l2p2, Vector3 &hitpoint);
        static bool RayIntersectsTriangle(const Vector3 &origin, const Vector3 &dir, const Vector3 &v0, const Vector3 &v1, const Vector3 &v2, float &intersection);
        static Vector3 SurfaceNormalFromIndices(const Vector3 &pA, const Vector3 &pB, const Vector3 &pC);
    };
};

#endif