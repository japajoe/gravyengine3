#ifndef GRAVYENGINE_TERRAINCOLLIDER_HPP
#define GRAVYENGINE_TERRAINCOLLIDER_HPP

#include "Collider.hpp"
#include "../../Rendering/Mesh.hpp"

namespace GravyEngine
{
    class TerrainCollider : public Collider
    {
    public:
        TerrainCollider();
        void SetMesh(Mesh *mesh);
        Mesh *GetMesh() const;
        void SetWidth(uint32_t width);
        uint32_t GetWidth() const;
        void SetDepth(uint32_t depth);
        uint32_t GetDepth() const;
    protected:
        Mesh *mesh;
        uint32_t width;
        uint32_t depth;
    };
};

#endif