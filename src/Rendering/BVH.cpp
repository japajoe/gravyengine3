#include "BVH.hpp"

namespace GravyEngine
{
    BVHTriangle::BVHTriangle()
    {

    }
    
    BVHTriangle::BVHTriangle(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3)
    {
        this->v1 = v1;
        this->v2 = v2;
        this->v3 = v3;
    }

    BVH::BVH()
    {

    }

    BVH::BVH(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices)
    {
        for(size_t i = 0; i < vertices.size(); i++)
        {
            bounds.Grow(vertices[i].position);
        }

        triangles.resize(indices.size() / 3);

        for(size_t i = 0; i < indices.size(); i++)
        {
            Vector3 a = vertices[indices[i+0]].position;
            Vector3 b = vertices[indices[i+1]].position;
            Vector3 c = vertices[indices[i+2]].position;
            triangles[i] = BVHTriangle(a, b, c);
        }
    }
};