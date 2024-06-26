#ifndef GRAVYENGINE_BVH_HPP
#define GRAVYENGINE_BVH_HPP

#include "BoundingBox.hpp"
#include "Vertex.hpp"
#include "../System/Numerics/Vector3.hpp"
#include <vector>
#include <cstdint>
#include <cstdlib>

namespace GravyEngine
{
    struct BVHTriangle
    {
        Vector3 v1;
        Vector3 v2;
        Vector3 v3;
        BVHTriangle();
        BVHTriangle(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3);
    };

    class BVH
    {
    public:
        BVH();
        BVH(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices);
    private:
        BoundingBox bounds;
        std::vector<BVHTriangle> triangles;
    };
};

#endif