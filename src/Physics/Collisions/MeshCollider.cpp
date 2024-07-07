#include "MeshCollider.hpp"

namespace GravyEngine
{
    MeshCollider::MeshCollider() : Collider()
    {
        this->type = ColliderType::Mesh;
        this->center = Vector3(0, 0, 0);
        this->mesh = nullptr;
        this->indexVertexArray = nullptr;
        SetName("MeshCollider");
    }

    void MeshCollider::SetMesh(Mesh *mesh)
    {
        this->mesh = mesh;
    }

    Mesh *MeshCollider::GetMesh() const
    {
        return mesh;
    }
};