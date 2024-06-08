#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "Buffers/UniformBufferObject.hpp"
#include <vector>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <memory>

namespace GravyEngine
{
    class CascadedShadowMap;
    class GameObject;
    class Renderer;
    class Shader;
    class DepthMaterial;

    class Graphics
    {
    friend class Application;
    public:
        static void AddRenderer(GameObject *obj);
        static void RemoveRenderer(GameObject *obj);
        static UniformBufferObject *FindUniformBuffer(const std::string &name);
    private:
        static std::vector<Renderer*> renderers;
        static std::vector<std::unique_ptr<UniformBufferObject>> uniformBuffers;
        static std::unique_ptr<CascadedShadowMap> cascadedShadowMap;
        static std::unique_ptr<DepthMaterial> depthMaterial;
        static void Initialize();
        static void Deinitialize();
        static void OnRender();
        static void RenderShadowMap();
        static void RenderScene();
        static void CreateTextures();
        static void CreateShaders();
        static void CreateMeshes();
        static void CreateUniformBuffers();
        static void CreateShadowMap();
        static void DestroyTextures();
        static void DestroyShaders();
        static void DestroyMeshes();
        static void DestroyUniformBuffers();
        static void DestroyShadowMap();
        static UniformBufferObject *CreateUniformBuffer(const std::string &name, uint32_t bindingIndex, size_t bufferSize, const std::vector<Shader*> &shaders);
    };
};

#endif