#ifndef GRAVYENGINE_MESHCOLLIDER_HPP
#define GRAVYENGINE_MESHCOLLIDER_HPP

#include "Collider.hpp"
#include "../../Rendering/Mesh.hpp"

class btTriangleIndexVertexArray;

namespace GravyEngine
{
    class MeshCollider : public Collider
    {
    public:
        MeshCollider();
        void SetMesh(Mesh *mesh);
        Mesh *GetMesh() const;
    protected:
        Mesh *mesh;
        btTriangleIndexVertexArray *indexVertexArray;
    };
};

#endif