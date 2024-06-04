#ifndef GRAVYENGINE_MATERIAL_HPP
#define GRAVYENGINE_MATERIAL_HPP

#include "../Core/Object.hpp"
#include "Shader.hpp"

namespace GravyEngine
{
    class Material : public Object
    {
    public:
        Material();
        virtual ~Material();
        void SetShader(Shader *shader);
        Shader *GetShader() const;
        void Use();
        virtual void UpdateUniforms() = 0;
    protected:
        Shader *pShader;
    };
};

#endif