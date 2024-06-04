#include "Material.hpp"

namespace GravyEngine
{
    Material::Material() : Object()
    {
        pShader = nullptr;
    }

    Material::~Material()
    {

    }

    void Material::SetShader(Shader *shader)
    {
        pShader = shader;
    }

    Shader *Material::GetShader() const
    {
        return pShader;
    }

    void Material::Use()
    {
        if(pShader)
            pShader->Use();
    }
};