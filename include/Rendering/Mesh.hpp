#ifndef GRAVYENGINE_MESH_HPP
#define GRAVYENGINE_MESH_HPP

#include "../System/Numerics/Vector3.hpp"
#include "../System/Numerics/Vector4.hpp"
#include "../Core/Object.hpp"
#include "Buffers/ElementBufferObject.hpp"
#include "Buffers/VertexArrayObject.hpp"
#include "Buffers/VertexBufferObject.hpp"
#include "Vertex.hpp"
#include <vector>
#include <cstdint>
#include <string>

namespace GravyEngine
{
    class Mesh : public Object
    {
    public:
        Mesh();
        Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices, bool calculateNormals);
        std::vector<Vertex> &GetVertices();
        std::vector<uint32_t> &GetIndices();
        size_t GetVerticesCount() const;
        size_t GetIndicesCount() const;
        VertexArrayObject *GetVAO();
        VertexBufferObject *GetVBO();
        ElementBufferObject *GetEBO();
        void Generate();
        void Delete();
        void RecalculateNormals();
    private:
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        size_t sizeOfVertices;
        size_t sizeOfIndices;
        VertexArrayObject VAO;
        VertexBufferObject VBO;
        ElementBufferObject EBO;
        Vector3 SurfaceNormalFromIndices(int32_t indexA, int32_t indexB, int32_t indexC);
    };

    class MeshGenerator
    {
    public:
        static Mesh Combine(const std::vector<Mesh*> &meshes);
        static Mesh CreateCapsule(const Vector3 &scale);
        static Mesh CreateCube(const Vector3 &scale);
        static Mesh CreatePlane(const Vector3 &scale);
        static Mesh CreateSkybox(const Vector3 &scale);
        static Mesh CreateQuad(const Vector3 &scale);
        static Mesh CreateSphere(const Vector3 &scale);
        static Mesh CreateHemisphere(const Vector3 &scale);
        static Mesh CreateTerrain(uint32_t width, uint32_t height, const Vector3 &scale);
    private:
        static void SetScale(std::vector<Vertex> &vertices, const Vector3 &scale);
    };
};

#endif