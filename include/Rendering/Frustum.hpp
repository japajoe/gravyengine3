#ifndef GRAVYENGINE_FRUSTUM_HPP
#define GRAVYENGINE_FRUSTUM_HPP

#include "BoundingBox.hpp"
#include "../System/Numerics/Matrix4.hpp"
#include "../System/Numerics/Vector3.hpp"
#include "../System/Numerics/Vector4.hpp"

namespace GravyEngine
{
    class Frustum
    {
    public:
        Frustum();
        void Initialize(const Matrix4 &viewProjection);
        bool Contains(const BoundingBox &bounds);
    private:
        Vector4 planes[6];
    };
};

#endif