#ifndef GRAVYENGINE_BOUNDINGBOX_HPP
#define GRAVYENGINE_BOUNDINGBOX_HPP

#include "../System/Numerics/Vector3.hpp"

namespace GravyEngine
{
    class BoundingBox
    {
    public:
        BoundingBox();
        void Grow(const Vector3 &point);
        Vector3 GetMin() const;
        Vector3 GetMax() const;
        Vector3 GetCenter() const;
        void Clear();
    private:
        Vector3 min;
        Vector3 max;
        Vector3 center;
    };
};

#endif