#ifndef GRAVYENGINE_MATERIAL_HPP
#define GRAVYENGINE_MATERIAL_HPP

#include "../Core/Camera.hpp"
#include "../Core/Object.hpp"
#include "../Core/Transform.hpp"
#include "Shader.hpp"

namespace GravyEngine
{
    class Material : public Object
    {
    public:
        Material();
        Material(Shader *shader);
        void SetShader(Shader *shader);
        Shader *GetShader() const;
        virtual void Use(Transform *transform, Camera *camera) = 0;
    protected:
        Shader *pShader;
    };
};

#endif