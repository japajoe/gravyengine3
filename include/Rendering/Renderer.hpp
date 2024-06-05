#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../Core/Component.hpp"
#include "Material.hpp"
#include <memory>
#include <type_traits>

namespace GravyEngine
{
    class Renderer : public Component
    {
    public:
        Renderer();
        virtual ~Renderer();
        virtual void OnRender() = 0;
        void SetMaterial(const std::shared_ptr<Material> &material); 

        template<typename T>
        T *GetMaterial() const
        {
            static_assert(std::is_base_of<Material, T>::value, "GetMaterial parameter must derive from Material");

            if(pMaterial == nullptr)
                return nullptr;

            T *ptr = dynamic_cast<T*>(pMaterial.get());
            
            if(ptr)
                return ptr;
            return nullptr;
        }

    protected:
        std::shared_ptr<Material> pMaterial;
    };
};

#endif