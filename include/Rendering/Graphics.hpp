#ifndef GRAVYENGINE_GRAPHICS_HPP
#define GRAVYENGINE_GRAPHICS_HPP

#include "Buffers/FrameBufferObject.hpp"
#include <vector>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <queue>

namespace GravyEngine
{
    class CascadedShadowMap;
    class GameObject;
    class Renderer;
    class Shader;
    class DepthMaterial;
    class Graphics2D;

    struct CompareRendererOrder 
    {
        bool operator()(const Renderer *lhs, const Renderer *rhs) const;
    };

    class Graphics
    {
    friend class Application;
    public:
        static void AddRenderer(Renderer *renderer);
        static void RemoveRenderer(Renderer *renderer);
        static Renderer *GetRendererByIndex(size_t index);
        static FrameBufferObject *GetFrameBuffer();
        static Graphics2D *GetGraphics2D();
    private:
        static std::priority_queue<Renderer*, std::vector<Renderer*>, CompareRendererOrder> renderQueue;
        static std::vector<Renderer*> renderers;
        static std::unique_ptr<Graphics2D> graphics2D;
        static std::unique_ptr<CascadedShadowMap> cascadedShadowMap;
        static std::unique_ptr<DepthMaterial> depthMaterial;
        static std::vector<FrameBufferObject> framebuffers;
        static void Initialize();
        static void Deinitialize();
        static void OnResize(uint32_t width, uint32_t height);
        static void OnRender();
        static void RenderShadowMap();
        static void RenderScene();
        static void CreateShadowMap();
        static void DestroyShadowMap();
        static void DestroyRenderers();
    };
};

#endif