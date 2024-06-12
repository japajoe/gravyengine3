#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../Core/Component.hpp"
#include "../Core/Camera.hpp"
#include "../External/glad/glad.h"
#include "Material.hpp"
#include <memory>
#include <type_traits>
#include <cstdint>

namespace GravyEngine
{
    struct RenderSettings
    {
        bool wireframe;
        bool depthTest;
        bool cullFace;
        bool alphaBlend;
        GLenum depthFunc;
        RenderSettings();
    };

    class Renderer : public Component
    {
    public:
        Renderer();
        virtual void OnRender() = 0;
        virtual void OnRender(Material *material, Camera *camera) = 0;
        void SetCastShadows(bool castShadows);
        bool GetCastShadows() const;
        void SetReceiveShadows(bool receiveShadows);
        bool GetReceiveShadows() const;
        void SetRenderOrder(uint32_t order);
        uint32_t GetRenderOrder() const;
    protected:
        bool castShadows;
        bool receiveShadows;
        uint32_t renderOrder;
        void OnDestroy() override;
    };
};

#endif