#ifndef GRAVYENGINE_GRAPHICS_HPP
#define GRAVYENGINE_GRAPHICS_HPP

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
    private:
        static std::priority_queue<Renderer*, std::vector<Renderer*>, CompareRendererOrder> renderQueue;
        static std::vector<Renderer*> renderers;
        static std::unique_ptr<CascadedShadowMap> cascadedShadowMap;
        static std::unique_ptr<DepthMaterial> depthMaterial;
        static void Initialize();
        static void Deinitialize();
        static void OnRender();
        static void RenderShadowMap();
        static void RenderScene();
        static void CreateShadowMap();
        static void DestroyShadowMap();
        static void DestroyRenderers();
    };
};

#endif