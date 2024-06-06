#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../Core/Component.hpp"
#include "../Core/Camera.hpp"
#include "../External/glad/glad.h"
#include "Material.hpp"
#include <memory>
#include <type_traits>

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
    protected:
        bool castShadows;
        bool receiveShadows;
        void OnDestroy() override;
    };
};

#endif