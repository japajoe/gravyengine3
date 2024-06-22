#ifndef GRAVYENGINE_RESOURCES_HPP
#define GRAVYENGINE_RESOURCES_HPP

#include "../Rendering/Texture2D.hpp"
#include "../Rendering/Texture2DArray.hpp"
#include "../Rendering/Shader.hpp"
#include "../Rendering/Mesh.hpp"
#include "../Rendering/Buffers/UniformBufferObject.hpp"
#include <unordered_map>
#include <string>

namespace GravyEngine
{
    class Resources
    {
    friend class Graphics;
    public:
        static Texture2D *AddTexture2D(const std::string &name, const Texture2D &texture);
        static Texture2DArray *AddTexture2DArray(const std::string &name, const Texture2DArray &texture);
        static Shader *AddShader(const std::string &name, const Shader &shader);
        static UniformBufferObject *AddUniformBuffer(const std::string &name, const UniformBufferObject &ubo);
        static Mesh *AddMesh(const std::string &name, const Mesh &mesh);
        static Texture2D *FindTexture2D(const std::string &name);
        static Texture2DArray *FindTexture2DArray(const std::string &name);
        static Shader *FindShader(const std::string &name);
        static UniformBufferObject *FindUniformBuffer(const std::string &name);
        static Mesh *FindMesh(const std::string &name);
        static void RemoveTexture2D(const std::string &name);
        static void RemoveTexture2DArray(const std::string &name);
        static void RemoveShader(const std::string &name);
        static void RemoveUniformBuffer(const std::string &name);
        static void RemoveMesh(const std::string &name);
    private:
        static std::unordered_map<std::string,Texture2D> textures2D;
        static std::unordered_map<std::string,Texture2DArray> textures2DArray;
        static std::unordered_map<std::string,Shader> shaders;
        static std::unordered_map<std::string,UniformBufferObject> ubos;
        static std::unordered_map<std::string,Mesh> meshes;
        static void Initialize();
        static void Deinitialize();
        static void CreateTextures();
        static void CreateShaders();
        static void CreateMeshes();
        static void CreateUniformBuffers();
    };
};

#endif