#ifndef GRAVYENGINE_SPHERECOLLIDER_HPP
#define GRAVYENGINE_SPHERECOLLIDER_HPP

#include "Collider.hpp"
#include <vector>

namespace GravyEngine
{
    class SphereCollider : public Collider
    {
    public:
        SphereCollider();
        void SetRadius(float radius);
        float GetRadius() const;
    protected:
        float radius;
    private:
        static std::vector<Vector4> unitSphere;
        static std::vector<Vector4> MakeUnitSphere(size_t len);
    };
};

#endif