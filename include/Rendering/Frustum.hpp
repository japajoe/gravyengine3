#ifndef GRAVYENGINE_FRUSTUM_HPP
#define GRAVYENGINE_FRUSTUM_HPP

#include "Plane.hpp"

namespace GravyEngine
{
    class Camera;

    struct Frustum
    {
        Plane topFace;
        Plane bottomFace;
        Plane rightFace;
        Plane leftFace;
        Plane farFace;
        Plane nearFace;
        Frustum();
        void Initialize(const Camera *camera);
    };
};

#endif