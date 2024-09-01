#include "TerrainCollider.hpp"

namespace GravyEngine
{
    TerrainCollider::TerrainCollider() : Collider()
    {
        this->type = ColliderType::Terrain;
        this->center = Vector3(0, 0, 0);
        this->mesh = nullptr;
        this->width = 0;
        this->depth = 0;
        this->scale = 1.0f;
        SetName("TerrainCollider");
    }

    void TerrainCollider::SetMesh(Mesh *mesh)
    {
        this->mesh = mesh;
    }

    Mesh *TerrainCollider::GetMesh() const
    {
        return mesh;
    }

    void TerrainCollider::SetWidth(uint32_t width)
    {
        this->width = width;
    }

    uint32_t TerrainCollider::GetWidth() const
    {
        return width;
    }

    void TerrainCollider::SetDepth(uint32_t depth)
    {
        this->depth = depth;
    }

    uint32_t TerrainCollider::GetDepth() const
    {
        return depth;
    }

    void TerrainCollider::SetScale(float scale)
    {
        this->scale = scale;
    }

    float TerrainCollider::GetScale() const
    {
        return scale;
    }
};