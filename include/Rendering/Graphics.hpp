#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "../Rendering/Buffers/UniformBufferObject.hpp"
#include <vector>
#include <string>
#include <cstdint>
#include <cstdlib>

namespace GravyEngine
{
    class GameObject;
    class Renderer;

    class Graphics
    {
    friend class Application;
    public:
        static void AddRenderer(GameObject *obj);
        static UniformBufferObject *FindUniformBuffer(const std::string &name);
    private:
        static std::vector<Renderer*> renderers;
        static std::vector<UniformBufferObject> uniformBuffers;
        static void Initialize();
        static void Deinitialize();
        static void OnRender();
        static void CreateTextures();
        static void CreateShaders();
        static void CreateMeshes();
        static void CreateUniformBuffers();
        static void DestroyTextures();
        static void DestroyShaders();
        static void DestroyMeshes();
        static void DestroyUniformBuffers();
        static UniformBufferObject *CreateUniformBuffer(const std::string &name, uint32_t bindingIndex, size_t bufferSize);
    };
};

#endif