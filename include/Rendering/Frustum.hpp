#ifndef GRAVYENGINE_FRUSTUM_HPP
#define GRAVYENGINE_FRUSTUM_HPP

#include "Plane.hpp"
#include "BoundingBox.hpp"
#include "../System/Numerics/Matrix4.hpp"
#include "../System/Numerics/Vector3.hpp"

namespace GravyEngine
{
    class Camera;

    class Frustum
    {
    public:
        Frustum();
        void Initialize(const Camera *camera);
        void Initialize(const Matrix4 &viewProjection);
        bool Contains(const BoundingBox &bounds);
    private:
        Vector4 planes[6];
        // Plane topFace;
        // Plane bottomFace;
        // Plane rightFace;
        // Plane leftFace;
        // Plane farFace;
        // Plane nearFace;
    };
};

#endif