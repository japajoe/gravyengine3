#ifndef GRAVYENGINE_MESH_HPP
#define GRAVYENGINE_MESH_HPP

#include <vector>
#include <cstdint>
#include "Vertex.hpp"
#include "../Core/Object.hpp"

namespace GravyEngine
{
    class Mesh : public Object
    {
    private:
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        Vector3 SurfaceNormalFromIndices(int32_t indexA, int32_t indexB, int32_t indexC);
    public:
        Mesh();
        Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices, bool calculateNormals);
        std::vector<Vertex> &GetVertices();
        std::vector<uint32_t> &GetIndices();
        size_t GetVerticesCount() const;
        size_t GetIndicesCount() const;
        void RecalculateNormals();
    };
};

#endif