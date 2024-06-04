#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../Core/Component.hpp"
#include "Material.hpp"

namespace GravyEngine
{
    class Renderer : public Component
    {
    public:
        Renderer();
        virtual ~Renderer();
        virtual void OnRender() = 0;
        void SetMaterial(Material *material); 
        Material *GetMaterial() const;
    protected:
        Material *pMaterial;        
    };
};

#endif