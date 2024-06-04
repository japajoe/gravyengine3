#include "Mesh.hpp"

namespace GravyEngine
{
    Mesh::Mesh() : Object()
    {
        SetName("Mesh");
    }

    Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices, bool calculateNormals) : Object()
    {
        this->vertices = vertices;
        this->indices = indices;

        if(calculateNormals)
            RecalculateNormals();
        
        SetName("Mesh");
    }

    std::vector<Vertex> &Mesh::GetVertices()
    {
        return vertices;
    }
    std::vector<uint32_t> &Mesh::GetIndices()
    {
        return indices;
    }

    size_t Mesh::GetVerticesCount() const
    {
        return vertices.size();
    }

    size_t Mesh::GetIndicesCount() const
    {
        return indices.size();
    }

    void Mesh::RecalculateNormals()
    {
        size_t triangleCount = indices.size() / 3;

        for (size_t i = 0; i < triangleCount; i++)
        {
            int normalTriangleIndex = i * 3;
            int vertexIndexA = (int)indices[normalTriangleIndex];
            int vertexIndexB = (int)indices[normalTriangleIndex + 1];
            int vertexIndexC = (int)indices[normalTriangleIndex + 2];
            
            Vector3 triangleNormal = SurfaceNormalFromIndices(vertexIndexA, vertexIndexB, vertexIndexC);

            Vertex &v1 = vertices[vertexIndexA];
            Vertex &v2 = vertices[vertexIndexB];
            Vertex &v3 = vertices[vertexIndexC];

            v1.normal += triangleNormal;
            v2.normal += triangleNormal;
            v3.normal += triangleNormal;

            v1.normal = Vector3f::Normalize(v1.normal);
            v2.normal = Vector3f::Normalize(v2.normal);
            v3.normal = Vector3f::Normalize(v3.normal);
        }
    }

    Vector3 Mesh::SurfaceNormalFromIndices(int indexA, int indexB, int indexC)
    {
        Vector3 pA = vertices[indexA].position;
        Vector3 pB = vertices[indexB].position;
        Vector3 pC = vertices[indexC].position;

        Vector3 sideAB = pB - pA;
        Vector3 sideAC = pC - pA;
        return Vector3f::Normalize(Vector3f::Cross(sideAB, sideAC));
    }
};