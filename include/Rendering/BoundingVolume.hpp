#ifndef GRAVYENGINE_BOUNDINGVOLUME_HPP
#define GRAVYENGINE_BOUNDINGVOLUME_HPP

#include "Frustum.hpp"
#include "Plane.hpp"
#include "../Core/Transform.hpp"

namespace GravyEngine
{
    class BoundingVolume
    {
    public:
        virtual bool IsOnFrustum(const Frustum &camFrustum, const Transform *transform) const = 0;
        virtual bool IsOnOrForwardPlane(const Plane& plane) const = 0;
        bool IsOnFrustum(const Frustum& camFrustum) const
        {
            return (IsOnOrForwardPlane(camFrustum.leftFace) &&
                IsOnOrForwardPlane(camFrustum.rightFace) &&
                IsOnOrForwardPlane(camFrustum.topFace) &&
                IsOnOrForwardPlane(camFrustum.bottomFace) &&
                IsOnOrForwardPlane(camFrustum.nearFace) &&
                IsOnOrForwardPlane(camFrustum.farFace));
        };
    };
};

#endif