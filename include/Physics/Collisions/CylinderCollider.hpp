#ifndef GRAVYENGINE_CYLINDEROLLIDER_HPP
#define GRAVYENGINE_CYLINDEROLLIDER_HPP

#include "Collider.hpp"

namespace GravyEngine
{
    class CylinderCollider : public Collider
    {
    public:
        CylinderCollider();
        void SetSize(const Vector3 &size);
        Vector3 GetSize() const;
    protected:
        Vector3 size;
    };
};

#endif