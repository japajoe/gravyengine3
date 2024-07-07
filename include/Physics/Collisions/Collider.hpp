#ifndef GRAVYENGINE_COLLIDER_HPP
#define GRAVYENGINE_COLLIDER_HPP

#include "../../Core/Component.hpp"
#include "../../System/Numerics/Vector2.hpp"
#include "../../System/Numerics/Vector3.hpp"
#include "../../System/Numerics/Vector4.hpp"
#include "../../System/Numerics/Quaternion.hpp"

namespace GravyEngine
{
    enum class ColliderType
    {
        Box,
        Capsule,
        Cylinder,
        Mesh,
        Sphere,
        Terrain,
        Wheel
    };

    class Collider : public Component
    {
    public:
        Collider();
        virtual ~Collider();
        ColliderType GetType() const;
        void SetCenter(const Vector3 &center);
        Vector3 GetCenter() const;
    protected:
        ColliderType type;
        Vector3 center;
    };
};

#endif