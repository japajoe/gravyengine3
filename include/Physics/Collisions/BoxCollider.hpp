#ifndef GRAVYENGINE_BOXCOLLIDER_HPP
#define GRAVYENGINE_BOXCOLLIDER_HPP

#include "Collider.hpp"

namespace GravyEngine
{
    class BoxCollider : public Collider
    {
    public:
        BoxCollider();
        void SetSize(const Vector3 &size);
        Vector3 GetSize() const;
    protected:
        Vector3 size;
    };
};

#endif