#include "DiffuseMaterial.hpp"
#include "../../External/glad/glad.h"

namespace GravyEngine
{
    DiffuseMaterial::DiffuseMaterial() : Material()
    {
        pShader = Shader::Find("Diffuse");

        if(pShader)
        {
            uModel = glGetUniformLocation(pShader->GetId(), "uModel");
            uModelInverted = glGetUniformLocation(pShader->GetId(), "uModelInverted");
            uMVP = glGetUniformLocation(pShader->GetId(), "uMVP");
            uDiffuseTexture = glGetUniformLocation(pShader->GetId(), "uDiffuseTexture");
            uUVOffset = glGetUniformLocation(pShader->GetId(), "uUVOffset");
            uUVScale = glGetUniformLocation(pShader->GetId(), "uUVScale");
        }
    }
    
    void DiffuseMaterial::UpdateUniforms()
    {
        
    }
};