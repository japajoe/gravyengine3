#include "Mesh.hpp"
#include "../Core/Debug.hpp"
#include "../System/Mathf.hpp"
#include "../External/glad/glad.h"

namespace GravyEngine
{
    Mesh::Mesh() : Object()
    {
        sizeOfVertices = 0;
        sizeOfIndices = 0;
        SetName("Mesh");
    }

    Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices, bool calculateNormals) : Object()
    {
        this->vertices = vertices;
        this->indices = indices;

        sizeOfVertices = this->vertices.size();
        sizeOfIndices = this->indices.size();

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

    VertexArrayObject *Mesh::GetVAO()
    {
        return &VAO;
    }

    VertexBufferObject *Mesh::GetVBO()
    {
        return &VBO;
    }

    ElementBufferObject *Mesh::GetEBO()
    {
        return &EBO;
    }

    void Mesh::Generate()
    {
        auto &vertices = GetVertices();
        auto &indices = GetIndices();

        if(VAO.GetId() == 0 && VBO.GetId() == 0 && EBO.GetId() == 0)
        {
            VAO.Generate();
            VAO.Bind();

            VBO.Generate();
            VBO.Bind();
            VBO.BufferData(vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

            VAO.EnableVertexAttribArray(0);
            VAO.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, position));

            VAO.EnableVertexAttribArray(1);
            VAO.VertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, normal));

            VAO.EnableVertexAttribArray(2);
            VAO.VertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, uv));

            if(indices.size() > 0)
            {
                EBO.Generate();
                EBO.Bind();                
                EBO.BufferData(indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
            }

            VAO.Unbind();
        }
        else
        {
            VBO.Bind();

            if(sizeOfVertices == vertices.size())
                VBO.BufferSubData(0, vertices.size() * sizeof(Vertex), vertices.data());
            else
                VBO.BufferData(vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

            sizeOfVertices = vertices.size();

            if(indices.size() > 0)
            {
                EBO.Bind();

                if(sizeOfIndices == indices.size())
                    EBO.BufferSubData(0, indices.size() * sizeof(GLuint), indices.data());
                else
                    EBO.BufferData(indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

                sizeOfIndices = indices.size();

                EBO.Unbind();
            }

            VBO.Unbind();
        }

        bounds.Clear();
        for(size_t i = 0; i < vertices.size(); i++)
        {
            bounds.Grow(vertices[i].position);
        }
    }

    void Mesh::Delete()
    {
        EBO.Delete();
        VBO.Delete();
        VAO.Delete();
    }

    size_t Mesh::GetVerticesCount() const
    {
        return vertices.size();
    }

    size_t Mesh::GetIndicesCount() const
    {
        return indices.size();
    }

    BoundingBox Mesh::GetBounds() const
    {
        return bounds;
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

    static Vector3 PointOnSpheroid(float radius, float height, float horizontalAngle, float verticalAngle)
    {
        float horizontalRadians = horizontalAngle * Mathf::Deg2Rad;
        float verticalRadians = verticalAngle * Mathf::Deg2Rad;
        float cosVertical = Mathf::Cos(verticalRadians);

        return Vector3(
            radius * Mathf::Sin(horizontalRadians) * cosVertical,
            height * Mathf::Sin(verticalRadians),
            radius * Mathf::Cos(horizontalRadians) * cosVertical);
    }

    static Vector3 PointOnSphere(float radius, float horizontalAngle, float verticalAngle)
    {
        return PointOnSpheroid(radius, radius, horizontalAngle, verticalAngle);
    }

    void MeshGenerator::SetScale(std::vector<Vertex> &vertices, const Vector3 &scale)
    {
        for(size_t i = 0; i < vertices.size(); i++)
            vertices[i].position *= scale;
    }

    Mesh MeshGenerator::Combine(const std::vector<Mesh*> &meshes)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        uint32_t indiceOffset = 0;

        for(size_t i = 0; i < meshes.size(); i++)
        {
            auto &mVertices = meshes[i]->GetVertices();

            for(size_t j = 0; j < mVertices.size(); j++)
            {
                vertices.push_back(mVertices[j]);
            }

            auto &mIndices = meshes[i]->GetIndices();

            for(size_t j = 0; j < mIndices.size(); j++)
            {
                indices.push_back(mIndices[j] + indiceOffset);
            }
            
            indiceOffset += (uint32_t)meshes[i]->GetVerticesCount();
        }

        Mesh mesh(vertices, indices, true);
        return mesh;
    }

    Mesh MeshGenerator::CreateCapsule(const Vector3 &scale)
    {
        float height = 2.0f;
        float radius = 0.5f;   
        int segments = 32;
        int rings = 8;
        float cylinderHeight = height - radius * 2;
        int vertexCount = 2 * rings * segments + 2;
        int triangleCount = 4 * rings * segments;
        float horizontalAngle = 360.0f / segments;
        float verticalAngle = 90.0f / rings;

        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        vertices.resize(vertexCount);
        indices.resize(3 * triangleCount);

        int vi = 2;
        int ti = 0;
        int topCapIndex = 0;
        int bottomCapIndex = 1;

        vertices[topCapIndex].position = Vector3(0, cylinderHeight / 2 + radius, 0);
        vertices[topCapIndex].normal = Vector3(0, 1, 0);
        vertices[bottomCapIndex].position = Vector3(0, -cylinderHeight / 2 - radius, 0);
        vertices[bottomCapIndex].normal = Vector3(0, -1, 0);

        for (int s = 0; s < segments; s++)
        {
            for (int r = 1; r <= rings; r++)
            {
                // Top cap vertex
                Vector3 normal = PointOnSphere(1, s*horizontalAngle, 90 - r * verticalAngle);
                Vector3 vertex = Vector3(radius*normal.x, radius * normal.y + cylinderHeight / 2, radius * normal.z);
                vertices[vi].position = vertex;
                vertices[vi].normal = normal;
                vi++;

                // Bottom cap vertex
                vertices[vi].position = Vector3(vertex.x, -vertex.y, vertex.z);
                vertices[vi].normal = Vector3(normal.x, -normal.y, normal.z);
                vi++;

                int top_s1r1 = vi - 2;
                int top_s1r0 = vi - 4;
                int bot_s1r1 = vi - 1;
                int bot_s1r0 = vi - 3;
                int top_s0r1 = top_s1r1 - 2 * rings;
                int top_s0r0 = top_s1r0 - 2 * rings;
                int bot_s0r1 = bot_s1r1 - 2 * rings;
                int bot_s0r0 = bot_s1r0 - 2 * rings;

                if (s == 0)
                {
                    top_s0r1 += vertexCount - 2;
                    top_s0r0 += vertexCount - 2;
                    bot_s0r1 += vertexCount - 2;
                    bot_s0r0 += vertexCount - 2;
                }

                // Create cap triangles
                if (r == 1)
                {
                    indices[3 * ti + 0] = topCapIndex;
                    indices[3 * ti + 1] = top_s0r1;
                    indices[3 * ti + 2] = top_s1r1;
                    ti++;

                    indices[3 * ti + 0] = bottomCapIndex;
                    indices[3 * ti + 1] = bot_s1r1;
                    indices[3 * ti + 2] = bot_s0r1;
                    ti++;
                }
                else
                {
                    indices[3 * ti + 0] = top_s1r0;
                    indices[3 * ti + 1] = top_s0r0;
                    indices[3 * ti + 2] = top_s1r1;
                    ti++;

                    indices[3 * ti + 0] = top_s0r0;
                    indices[3 * ti + 1] = top_s0r1;
                    indices[3 * ti + 2] = top_s1r1;
                    ti++;

                    indices[3 * ti + 0] = bot_s0r1;
                    indices[3 * ti + 1] = bot_s0r0;
                    indices[3 * ti + 2] = bot_s1r1;
                    ti++;

                    indices[3 * ti + 0] = bot_s0r0;
                    indices[3 * ti + 1] = bot_s1r0;
                    indices[3 * ti + 2] = bot_s1r1;
                    ti++;
                }
            }

            // Create side triangles
            int top_s1 = vi - 2;
            int top_s0 = top_s1 - 2 * rings;
            int bot_s1 = vi - 1;
            int bot_s0 = bot_s1 - 2 * rings;
            
            if (s == 0)
            {
                top_s0 += vertexCount - 2;
                bot_s0 += vertexCount - 2;
            }

            indices[3 * ti + 0] = top_s0;
            indices[3 * ti + 1] = bot_s1;
            indices[3 * ti + 2] = top_s1;
            ti++;

            indices[3 * ti + 0] = bot_s0;
            indices[3 * ti + 1] = bot_s1;
            indices[3 * ti + 2] = top_s0;
            ti++;
        }

        SetScale(vertices, scale);
        Mesh mesh(vertices, indices, true);
        mesh.SetName("Capsule");
        return mesh;
    }

    Mesh MeshGenerator::CreateCube(const Vector3 &scale)
    {
        std::vector<Vertex> vertices;
        vertices.resize(24);

        vertices[0].position = Vector3(0.5f, -0.5f, 0.5f);
        vertices[1].position = Vector3(-0.5f, -0.5f, 0.5f);
        vertices[2].position = Vector3(0.5f, 0.5f, 0.5f);
        vertices[3].position = Vector3(-0.5f, 0.5f, 0.5f);

        vertices[4].position = Vector3(0.5f, 0.5f, -0.5f);
        vertices[5].position = Vector3(-0.5f, 0.5f, -0.5f);
        vertices[6].position = Vector3(0.5f, -0.5f, -0.5f);
        vertices[7].position = Vector3(-0.5f, -0.5f, -0.5f);

        vertices[8].position = Vector3(0.5f, 0.5f, 0.5f);
        vertices[9].position = Vector3(-0.5f, 0.5f, 0.5f);
        vertices[10].position = Vector3(0.5f, 0.5f, -0.5f);
        vertices[11].position = Vector3(-0.5f, 0.5f, -0.5f);

        vertices[12].position = Vector3(0.5f, -0.5f, -0.5f);
        vertices[13].position = Vector3(0.5f, -0.5f, 0.5f);
        vertices[14].position = Vector3(-0.5f, -0.5f, 0.5f);
        vertices[15].position = Vector3(-0.5f, -0.5f, -0.5f);

        vertices[16].position = Vector3(-0.5f, -0.5f, 0.5f);
        vertices[17].position = Vector3(-0.5f, 0.5f, 0.5f);
        vertices[18].position = Vector3(-0.5f, 0.5f, -0.5f);
        vertices[19].position = Vector3(-0.5f, -0.5f, -0.5f);

        vertices[20].position = Vector3(0.5f, -0.5f, -0.5f);
        vertices[21].position = Vector3(0.5f, 0.5f, -0.5f);
        vertices[22].position = Vector3(0.5f, 0.5f, 0.5f);
        vertices[23].position = Vector3(0.5f, -0.5f, 0.5f);

        // UVS
        vertices[0].uv = Vector2(0.0f, 0.0f);
        vertices[1].uv = Vector2(1.0f, 0.0f);
        vertices[2].uv = Vector2(0.0f, 1.0f);
        vertices[3].uv = Vector2(1.0f, 1.0f);

        vertices[4].uv = Vector2(0.0f, 1.0f);
        vertices[5].uv = Vector2(1.0f, 1.0f);
        vertices[6].uv = Vector2(0.0f, 1.0f);
        vertices[7].uv = Vector2(1.0f, 1.0f);
        
        vertices[8].uv = Vector2(0.0f, 0.0f);
        vertices[9].uv = Vector2(1.0f, 0.0f);
        vertices[10].uv = Vector2(0.0f, 0.0f);
        vertices[11].uv = Vector2(1.0f, 0.0f);
        
        vertices[12].uv = Vector2(0.0f, 0.0f);
        vertices[13].uv = Vector2(0.0f, 1.0f);
        vertices[14].uv = Vector2(1.0f, 1.0f);
        vertices[15].uv = Vector2(1.0f, 0.0f);
        
        vertices[16].uv = Vector2(0.0f, 0.0f);
        vertices[17].uv = Vector2(0.0f, 1.0f);
        vertices[18].uv = Vector2(1.0f, 1.0f);
        vertices[19].uv = Vector2(1.0f, 0.0f);
        
        vertices[20].uv = Vector2(0.0f, 0.0f);
        vertices[21].uv = Vector2(0.0f, 1.0f);
        vertices[22].uv = Vector2(1.0f, 1.0f);
        vertices[23].uv = Vector2(1.0f, 0.0f);

        std::vector<GLuint> indices =
        {
            0, 2, 3,
            0, 3, 1,

            8, 4, 5,
            8, 5, 9,

            10, 6, 7,
            10, 7, 11,

            12, 13, 14,
            12, 14, 15,

            16, 17, 18,
            16, 18, 19,

            20, 21, 22,
            20, 22, 23
        };

        SetScale(vertices, scale);
        Mesh mesh(vertices, indices, true);
        mesh.SetName("Cube");
        return mesh;
    }

    Mesh MeshGenerator::CreatePlane(const Vector3 &scale)
    {
        std::vector<Vertex> vertices;
        vertices.resize(4);

        Vertex bottomLeft;
        Vertex bottomRight;
        Vertex topleft;
        Vertex topRight;

        topleft.position = Vector3(-0.5f, 0.0f, -0.5f);
        bottomLeft.position = Vector3(-0.5f, 0.0f, 0.5f);
        bottomRight.position = Vector3(0.5f, 0.0f, 0.5f);
        topRight.position = Vector3(0.5f, 0.0f, -0.5f);

        //topleft.uv = Vector2(0.0f, 1.0f);
        //bottomLeft.uv = Vector2(0.0f, 0.0f);
        //bottomRight.uv = Vector2(1.0f, 0.0f);
        //topRight.uv = Vector2(1.0f, 1.0f);

        topleft.uv = Vector2(0.0f, 0.0f);
        bottomLeft.uv = Vector2(0.0f, 1.0f);
        bottomRight.uv = Vector2(1.0f, 1.0f);
        topRight.uv = Vector2(1.0f, 0.0f);

        vertices[0] = topleft;
        vertices[1] = bottomLeft;
        vertices[2] = topRight;
        vertices[3] = bottomRight;

        std::vector<GLuint> indices =
        {
            0, 1, 2,
            2, 1, 3,
        };

        SetScale(vertices, scale);
        Mesh mesh(vertices, indices, true);
        mesh.SetName("Plane");
        return mesh;
    }

    Mesh MeshGenerator::CreateQuad(const Vector3 &scale)
    {
        std::vector<Vertex> vertices;
        vertices.resize(4);

        Vertex bottomLeft;
        Vertex bottomRight;
        Vertex topleft;
        Vertex topRight;

        topleft.position     = Vector3(0.0f, 1.0f, 0.0f);
        bottomLeft.position  = Vector3(0.0f, 0.0f, 0.0f);
        bottomRight.position = Vector3(1.0f, 0.0f, 0.0f);
        topRight.position    = Vector3(1.0f, 1.0f, 0.0f);

        topleft.uv           = Vector2(0.0f, 1.0f);
        bottomLeft.uv        = Vector2(0.0f, 0.0f);
        bottomRight.uv       = Vector2(1.0f, 0.0f);
        topRight.uv          = Vector2(1.0f, 1.0f);

        topleft.normal       = Vector3(0, 1, 0);
        bottomLeft.normal    = Vector3(0, 1, 0);
        bottomRight.normal   = Vector3(0, 1, 0);
        topRight.normal      = Vector3(0, 1, 0);

        vertices[0] = topleft;
        vertices[1] = bottomLeft;
        vertices[2] = topRight;
        vertices[3] = bottomRight;

        for(size_t i = 0; i < vertices.size(); i++)
        {
            vertices[i].position -= Vector3(0.5f, 0.5f, 0.0f);
        }

        std::vector<GLuint> indices =
        {
            0, 1, 2, 
            1, 3, 2
        };

        SetScale(vertices, scale);
        Mesh mesh(vertices, indices, true);
        mesh.SetName("Quad");
        return mesh;
    }

    Mesh MeshGenerator::CreateSkybox(const Vector3 &scale)
    {
        std::vector<Vertex> vertices;
        vertices.resize(36);

        vertices[0].position = Vector3(-1.0f, 1.0f, -1.0f);
        vertices[1].position = Vector3(-1.0f, -1.0f, -1.0f);
        vertices[2].position = Vector3(1.0f, -1.0f, -1.0f);
        vertices[3].position = Vector3(1.0f, -1.0f, -1.0f);
        vertices[4].position = Vector3(1.0f, 1.0f, -1.0f);
        vertices[5].position = Vector3(-1.0f, 1.0f, -1.0f);

        vertices[6].position = Vector3(-1.0f, -1.0f, 1.0f);
        vertices[7].position = Vector3(-1.0f, -1.0f, -1.0f);
        vertices[8].position = Vector3(-1.0f, 1.0f, -1.0f);
        vertices[9].position = Vector3(-1.0f, 1.0f, -1.0f);
        vertices[10].position = Vector3(-1.0f, 1.0f, 1.0f);
        vertices[11].position = Vector3(-1.0f, -1.0f, 1.0f);

        vertices[12].position = Vector3(1.0f, -1.0f, -1.0f);
        vertices[13].position = Vector3(1.0f, -1.0f, 1.0f);
        vertices[14].position = Vector3(1.0f, 1.0f, 1.0f);
        vertices[15].position = Vector3(1.0f, 1.0f, 1.0f);
        vertices[16].position = Vector3(1.0f, 1.0f, -1.0f);
        vertices[17].position = Vector3(1.0f, -1.0f, -1.0f);

        vertices[18].position = Vector3(-1.0f, -1.0f, 1.0f);
        vertices[19].position = Vector3(-1.0f, 1.0f, 1.0f);
        vertices[20].position = Vector3(1.0f, 1.0f, 1.0f);
        vertices[21].position = Vector3(1.0f, 1.0f, 1.0f);
        vertices[22].position = Vector3(1.0f, -1.0f, 1.0f);
        vertices[23].position = Vector3(-1.0f, -1.0f, 1.0f);

        vertices[24].position = Vector3(-1.0f, 1.0f, -1.0f);
        vertices[25].position = Vector3(1.0f, 1.0f, -1.0f);
        vertices[26].position = Vector3(1.0f, 1.0f, 1.0f);
        vertices[27].position = Vector3(1.0f, 1.0f, 1.0f);
        vertices[28].position = Vector3(-1.0f, 1.0f, 1.0f);
        vertices[29].position = Vector3(-1.0f, 1.0f, -1.0f);

        vertices[30].position = Vector3(-1.0f, -1.0f, -1.0f);
        vertices[31].position = Vector3(-1.0f, -1.0f, 1.0f);
        vertices[32].position = Vector3(1.0f, -1.0f, -1.0f);
        vertices[33].position = Vector3(1.0f, -1.0f, -1.0f);
        vertices[34].position = Vector3(-1.0f, -1.0f, 1.0f);
        vertices[35].position = Vector3(1.0f, -1.0f, 1.0f);

        for(size_t i = 0; i < vertices.size(); i+=6)
        {
            Vertex v1 = vertices[i+0];
            Vertex v2 = vertices[i+1];
            Vertex v3 = vertices[i+2];
            Vertex v4 = vertices[i+3];
            Vertex v5 = vertices[i+4];
            Vertex v6 = vertices[i+5];

            v1.uv = Vector2(0.0f, 0.0f);
            v2.uv = Vector2(0.0f, 1.0f);
            v3.uv = Vector2(1.0f, 1.0f);
            v4.uv = Vector2(1.0f, 1.0f);
            v5.uv = Vector2(1.0f, 0.0f);
            v6.uv = Vector2(0.0f, 0.0f);

            vertices[i+0] = v1;
            vertices[i+1] = v2;
            vertices[i+2] = v3;
            vertices[i+3] = v4;
            vertices[i+4] = v5;
            vertices[i+5] = v6;
        }
        
        std::vector<GLuint> indices;
        Mesh mesh(vertices, indices, false);
        mesh.SetName("Skybox");
        return mesh;
    }

    Mesh MeshGenerator::CreateSphere(const Vector3 &scale)
    {
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        size_t sectorCount = 72;
        size_t stackCount = 24;
        float radius = 0.5f;

        float x, y, z, xy;                              // vertex position
        float lengthInv = 1.0f / radius;    // vertex normal
        float s, t;                                     // vertex texCoord

        float sectorStep = 2 * Mathf::PI / sectorCount;
                
        float stackStep = Mathf::PI / stackCount;
        float sectorAngle, stackAngle;

        for(size_t i = 0; i <= stackCount; ++i)
        {
            stackAngle = Mathf::PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            xy = radius* Mathf::Cos(stackAngle);             // r * cos(u)
            z = radius* Mathf::Sin(stackAngle);              // r * sin(u)

            // add (sectorCount+1) vertices per stack
            // the first and last vertices have same position and normal, but different tex coords
            for(size_t j = 0; j <= sectorCount; ++j)
            {
                Vertex v;

                sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                // vertex position (x, y, z)
                x = xy * Mathf::Cos(sectorAngle);             // r * cos(u) * cos(v)
                y = xy * Mathf::Sin(sectorAngle);             // r * cos(u) * sin(v)          
                v.position = Vector3(x, y, z);

                // vertex tex coord (s, t) range between [0, 1]
                s = (float) j / sectorCount;
                t = (float) i / stackCount;          
                v.uv = Vector2(s, t);
                
                vertices.push_back(v);
            }
        }

        GLuint k1, k2;
        for(GLuint i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);     // beginning of current stack
            k2 = k1 + sectorCount + 1;      // beginning of next stack

            for(int j = 0; j<sectorCount; ++j, ++k1, ++k2)
            {
                // 2 triangles per sector excluding first and last stacks
                // k1 => k2 => k1+1
                if(i != 0)
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                // k1+1 => k2 => k2+1
                if(i != (stackCount-1))
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }

        SetScale(vertices, scale);
        Mesh mesh(vertices, indices, true);
        mesh.SetName("Sphere");
        return mesh;
    }

    Mesh MeshGenerator::CreateHemisphere(const Vector3 &scale)
    {
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        size_t sectorCount = 72;
        size_t stackCount = 24;
        float radius = 0.5f;

        float x, y, z, xy;                              // vertex position
        float lengthInv = 1.0f / radius;    // vertex normal
        float s, t;                                     // vertex texCoord

        float sectorStep = 2 * Mathf::PI / sectorCount;
        
        sectorStep = Mathf::PI / sectorCount;
        
        float stackStep = Mathf::PI / stackCount;
        float sectorAngle, stackAngle;

        for(size_t i = 0; i <= stackCount; ++i)
        {
            stackAngle = Mathf::PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            xy = radius* Mathf::Cos(stackAngle);             // r * cos(u)
            z = radius* Mathf::Sin(stackAngle);              // r * sin(u)

            // add (sectorCount+1) vertices per stack
            // the first and last vertices have same position and normal, but different tex coords
            for(size_t j = 0; j <= sectorCount; ++j)
            {
                Vertex v;

                sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                // vertex position (x, y, z)
                x = xy * Mathf::Cos(sectorAngle);             // r * cos(u) * cos(v)
                y = xy * Mathf::Sin(sectorAngle);             // r * cos(u) * sin(v)          
                v.position = Vector3(x, y, z);

                // vertex tex coord (s, t) range between [0, 1]
                s = (float) j / sectorCount;
                t = (float) i / stackCount;          
                v.uv = Vector2(s, t);
                
                vertices.push_back(v);
            }
        }

        GLuint k1, k2;
        for(GLuint i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);     // beginning of current stack
            k2 = k1 + sectorCount + 1;      // beginning of next stack

            for(int j = 0; j<sectorCount; ++j, ++k1, ++k2)
            {
                // 2 triangles per sector excluding first and last stacks
                // k1 => k2 => k1+1
                if(i != 0)
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                // k1+1 => k2 => k2+1
                if(i != (stackCount-1))
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }

        SetScale(vertices, scale);
        Mesh mesh(vertices, indices, true);
        mesh.SetName("Sphere");
        return mesh;
    }

    Mesh MeshGenerator::CreateTerrain(uint32_t width, uint32_t height, const Vector3 &scale)
    {
        width += 1;
        height += 1;

        uint32_t verticesPerLine = (uint32_t)((width - 1) / 2);

        float topLeftX = 0;
        float topLeftZ = 0;

        uint32_t numVertices = verticesPerLine * height;
        uint32_t numIndices = (verticesPerLine - 1) * (verticesPerLine - 1) * 6;
        uint32_t vertexIndex = 0;
        int32_t triangleIndex = 0;

        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        vertices.resize(numVertices);
        indices.resize(numIndices);

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                vertices[vertexIndex].position = Vector3(topLeftX + x, 0.0f, topLeftZ - y);
                vertices[vertexIndex].uv = Vector2((float)x / (width - 1.0f), (float)y / (height - 1.0f));

                if (x < width - 1 && y < height - 1)
                {
                    indices[triangleIndex + 0] = vertexIndex;
                    indices[triangleIndex + 1] = vertexIndex + verticesPerLine + 1;
                    indices[triangleIndex + 2] = vertexIndex + verticesPerLine;

                    indices[triangleIndex + 3] = vertexIndex + verticesPerLine + 1;
                    indices[triangleIndex + 4] = vertexIndex;
                    indices[triangleIndex + 5] = vertexIndex + 1;
                    triangleIndex += 6;
                }
                vertexIndex++;
            }
        }

        SetScale(vertices, scale);
        Mesh mesh(vertices, indices, true);
        mesh.SetName("Terrain");
        return mesh;
    }
};