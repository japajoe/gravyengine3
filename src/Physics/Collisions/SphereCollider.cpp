#include "SphereCollider.hpp"
#include "../../System/Mathf.hpp"

namespace GravyEngine
{
    std::vector<Vector4> SphereCollider::unitSphere;

    SphereCollider::SphereCollider() : Collider()
    {
        this->type = ColliderType::Sphere;
        this->center = Vector3(0, 0, 0);
        this->radius = 0.5f;
        SetName("SphereCollider");

        if(unitSphere.size() == 0)
            unitSphere = MakeUnitSphere(64);
    }

    void SphereCollider::SetRadius(float radius)
    {
        this->radius = radius;
    }

    float SphereCollider::GetRadius() const
    {
        return radius;
    }

    std::vector<Vector4> SphereCollider::MakeUnitSphere(size_t len)
    {
        if(len < 2)
            len = 2;
        std::vector<Vector4> v;
        v.resize(len * 3);

        for (size_t i = 0; i < len; i++)
        {
            float f = i / (float)len;
            float c = Mathf::Cos(f * (float)(Mathf::PI * 2.0));
            float s = Mathf::Sin(f * (float)(Mathf::PI * 2.0));
            v[0 * len + i] = Vector4(c, s, 0, 1);
            v[1 * len + i] = Vector4(0, c, s, 1);
            v[2 * len + i] = Vector4(s, 0, c, 1);
        }
        return v;
    }
};