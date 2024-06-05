#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../Core/Component.hpp"
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
        virtual ~Renderer();
        virtual void OnRender() = 0;
    };
};

#endif