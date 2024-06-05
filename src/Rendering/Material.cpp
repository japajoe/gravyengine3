#include "Material.hpp"

namespace GravyEngine
{
    Material::Material() : Object()
    {
        pShader = nullptr;
    }

    Material::Material(Shader *shader) : Object()
    {
        pShader = shader;
    }

    void Material::SetShader(Shader *shader)
    {
        pShader = shader;
    }

    Shader *Material::GetShader() const
    {
        return pShader;
    }
};