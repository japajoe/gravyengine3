#ifdef GRAVY_ENABLE_ASSIMP
#include "ModelImporter.hpp"
#include "Vertex.hpp"
#include "Mesh.hpp"
#include "MeshRenderer.hpp"
#include "Materials/DiffuseMaterial.hpp"
#include "../Core/Resources.hpp"
#include "../Core/Debug.hpp"
#include "../System/IO/File.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

namespace GravyEngine
{    
    static Matrix4 ToMatrix4(aiMatrix4x4 matrix)
    {
        matrix.Transpose();
        Matrix4 mat;
        mat[0][0] = matrix[0][0];
        mat[0][1] = matrix[0][1];
        mat[0][2] = matrix[0][2];
        mat[0][3] = matrix[0][3];

        mat[1][0] = matrix[1][0];
        mat[1][1] = matrix[1][1];
        mat[1][2] = matrix[1][2];
        mat[1][3] = matrix[1][3];

        mat[2][0] = matrix[2][0];
        mat[2][1] = matrix[2][1];
        mat[2][2] = matrix[2][2];
        mat[2][3] = matrix[2][3];

        mat[3][0] = matrix[3][0];
        mat[3][1] = matrix[3][1];
        mat[3][2] = matrix[3][2];
        mat[3][3] = matrix[3][3];
        return mat;
    }

    GameObject *ModelImporter::LoadFromFile(const std::string &filepath, unsigned int modelFlags, const Vector3 &scale, bool flipYZ)
    {
        if(!File::Exists(filepath))
        {
            Debug::WriteLog("Error loading model: file does not exist: %s", filepath.c_str());
            return nullptr;
        }

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(filepath, modelFlags);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            Debug::WriteLog("Error loading model: %s", importer.GetErrorString());
            return nullptr;
        }

        GameObject *model = GameObject::Create();

        ProcessNode(model, scene->mRootNode, scene, scale, flipYZ);

        return model;
    }

    GameObject *ModelImporter::LoadFromMemory(const void *memory, size_t size, unsigned int modelFlags, const Vector3 &scale, bool flipYZ)
    {
        if(size == 0)
        {
            Debug::WriteLog("Error loading model: size is 0");
            return nullptr;
        }

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFileFromMemory(memory, size, modelFlags);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            Debug::WriteLog("Error loading model: %s", importer.GetErrorString());
            return nullptr;
        }

        GameObject *model = GameObject::Create();

        ProcessNode(model, scene->mRootNode, scene, scale, flipYZ);

        return model;
    }

    std::vector<std::shared_ptr<Mesh>> ModelImporter::LoadMeshesFromFile(const std::string &filepath, unsigned int modelFlags, const Vector3 &scale, bool flipYZ)
    {
        std::vector<std::shared_ptr<Mesh>> meshes;

        if(!File::Exists(filepath))
        {
            Debug::WriteLog("Error loading mesh: file does not exist: %s", filepath.c_str());
            return meshes;
        }

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(filepath, modelFlags);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            Debug::WriteLog("Error loading mesh: %s", importer.GetErrorString());
            return meshes;
        }

        for(size_t i = 0; i < scene->mNumMeshes; i++)
        {
            aiMesh * aMesh = scene->mMeshes[i];
            std::vector<Vertex> vertices;
            vertices.resize(aMesh->mNumVertices);

            if(aMesh->HasTextureCoords(0))
            {
                for(size_t i = 0; i < aMesh->mNumVertices; i++)
                {
                    auto pos = aMesh->mVertices[i];
                    auto nrm = aMesh->mNormals[i];
                    auto uv = aMesh->mTextureCoords[0][i];

                    if(flipYZ)
                    {
                        float y = pos.z;
                        float z = pos.y;
                        pos.y = y;
                        pos.z = z;
                    }

                    vertices[i].position = Vector3(pos.x, pos.y, pos.z) * scale;
                    vertices[i].normal = Vector3(nrm.x, nrm.y, nrm.z);
                    vertices[i].uv = Vector2(uv.x, uv.y);
                }
            }
            else
            {
                for(size_t i = 0; i < aMesh->mNumVertices; i++)
                {
                    auto pos = aMesh->mVertices[i];
                    auto nrm = aMesh->mNormals[i];

                    if(flipYZ)
                    {
                        float y = pos.z;
                        float z = pos.y;
                        pos.y = y;
                        pos.z = z;
                    }

                    vertices[i].position = Vector3(pos.x, pos.y, pos.z) * scale;
                    vertices[i].normal = Vector3(nrm.x, nrm.y, nrm.z);
                    vertices[i].uv = Vector2(0.0f, 0.0f);
                }
            }

            std::vector<GLuint> indices;

            for(size_t i = 0; i < aMesh->mNumFaces; i++)
            {
                indices.push_back(aMesh->mFaces[i].mIndices[0]);
                indices.push_back(aMesh->mFaces[i].mIndices[1]);
                indices.push_back(aMesh->mFaces[i].mIndices[2]);
            }

            auto mesh = std::make_shared<Mesh>(vertices, indices, false);
            meshes.push_back(mesh);
        }

        return meshes;
    }

    void ModelImporter::ProcessNode(GameObject *parent, const aiNode* node, const aiScene* scene, const Vector3 &scale, bool flipYZ)
    {
        auto transformation = ToMatrix4(node->mTransformation);
        parent->GetTransform()->SetPosition(Matrix4f::ExtractTranslation(transformation) * scale);
        parent->GetTransform()->SetRotation(Matrix4f::ExtractRotation(transformation));
        parent->GetTransform()->SetScale(Matrix4f::ExtractScale(transformation));

        MeshRenderer *renderer = nullptr;
        
        if(node->mNumMeshes > 0)
        {
            std::string name(node->mName.C_Str());
            parent->SetName(name);
            renderer = parent->AddComponent<MeshRenderer>();
        }

        auto texture = Resources::FindTexture2D("Default");

        // Process all meshes in the current node
        for (size_t i = 0; i < node->mNumMeshes; i++) 
        {
            aiMesh* aMesh = scene->mMeshes[node->mMeshes[i]];

            std::vector<Vertex> vertices;
            vertices.resize(aMesh->mNumVertices);
            
            auto transformation = node->mTransformation * node->mParent->mTransformation;

            if(aMesh->HasTextureCoords(0))
            {
                for(size_t i = 0; i < aMesh->mNumVertices; i++)
                {
                    auto pos = aMesh->mVertices[i];
                    auto nrm = aMesh->mNormals[i];
                    auto uv = aMesh->mTextureCoords[0][i];

                    if(flipYZ)
                    {
                        float y = pos.z;
                        float z = pos.y;
                        pos.y = y;
                        pos.z = z;
                    }

                    vertices[i].position = Vector3(pos.x, pos.y, pos.z) * scale;
                    vertices[i].normal = Vector3(nrm.x, nrm.y, nrm.z);
                    vertices[i].uv = Vector2(uv.x, uv.y);
                }
            }
            else
            {
                for(size_t i = 0; i < aMesh->mNumVertices; i++)
                {
                    auto pos = aMesh->mVertices[i];
                    auto nrm = aMesh->mNormals[i];

                    if(flipYZ)
                    {
                        float y = pos.z;
                        float z = pos.y;
                        pos.y = y;
                        pos.z = z;
                    }

                    vertices[i].position = Vector3(pos.x, pos.y, pos.z) * scale;
                    vertices[i].normal = Vector3(nrm.x, nrm.y, nrm.z);
                    vertices[i].uv = Vector2(0.0f, 0.0f);
                }
            }

            std::vector<GLuint> indices;

            for(size_t i = 0; i < aMesh->mNumFaces; i++)
            {
                indices.push_back(aMesh->mFaces[i].mIndices[0]);
                indices.push_back(aMesh->mFaces[i].mIndices[1]);
                indices.push_back(aMesh->mFaces[i].mIndices[2]);
            }

            uint32_t materialIndex = aMesh->mMaterialIndex;
            aiMaterial* aMaterial = scene->mMaterials[materialIndex];

            auto mesh = std::make_shared<Mesh>(vertices, indices, false);
            mesh->Generate();
            mesh->SetName(aMaterial->GetName().C_Str());

            auto material = std::make_shared<DiffuseMaterial>();
            material->SetName(aMaterial->GetName().C_Str());
            material->SetDiffuseTexture(texture);
            
            renderer->Add(mesh, material);
        }

        // Process all child nodes
        for (size_t i = 0; i < node->mNumChildren; i++) 
        {
            auto child = GameObject::Create();
            child->SetName(std::string(node->mChildren[i]->mName.C_Str()));
            child->GetTransform()->SetParent(parent->GetTransform());
            ProcessNode(child, node->mChildren[i], scene, scale, flipYZ);
        }
    }
};
#endif