#include "DiffuseMaterial.hpp"
#include "../../System/Numerics/Matrix3.hpp"
#include "../../System/Numerics/Matrix4.hpp"
#include "../../System/Numerics/Vector2.hpp"
#include "../../System/Numerics/Vector3.hpp"
#include "../../External/glad/glad.h"

namespace GravyEngine
{
    DiffuseMaterial::DiffuseMaterial() : Material()
    {
        pDiffuseTexture = nullptr;
        diffuseColor = Color::White();

        pShader = Shader::Find("Diffuse");

        if(pShader)
        {
            uModel = glGetUniformLocation(pShader->GetId(), "uModel");
            uModelInverted = glGetUniformLocation(pShader->GetId(), "uModelInverted");
            uMVP = glGetUniformLocation(pShader->GetId(), "uMVP");
            uDiffuseTexture = glGetUniformLocation(pShader->GetId(), "uDiffuseTexture");
            uDiffuseColor = glGetUniformLocation(pShader->GetId(), "uDiffuseColor");
            uUVOffset = glGetUniformLocation(pShader->GetId(), "uUVOffset");
            uUVScale = glGetUniformLocation(pShader->GetId(), "uUVScale");
        }

        SetName("DiffuseMaterial");
    }
    
    void DiffuseMaterial::Use(Transform *transform, Camera *camera)
    {
        if(!pShader)
            return;
        if(!camera)
            return;
        if(!transform)
            return;

        Matrix4 projection = camera->GetProjectionMatrix();
        Matrix4 view = camera->GetViewMatrix();
        Matrix4 model = transform->GetModelMatrix();
        Matrix4 MVP = projection * view * model;
        Matrix3 modelInverted = glm::inverse(glm::transpose(glm::mat3(model)));
        Vector2 uvOffset(0, 0);
        Vector2 uvScale(1, 1);

        pShader->Use();

        pShader->SetMat4(uModel, glm::value_ptr(model));
        pShader->SetMat4(uModelInverted, glm::value_ptr(modelInverted));
        pShader->SetMat4(uMVP, glm::value_ptr(MVP));
        pShader->SetFloat2(uUVOffset, glm::value_ptr(uvOffset));
        pShader->SetFloat2(uUVScale, glm::value_ptr(uvScale));

        if(pDiffuseTexture != nullptr)
        {
            uint32_t unit = 0;
            pDiffuseTexture->Bind(unit);
            pShader->SetInt(uDiffuseTexture, unit);
            unit++;
        }

        pShader->SetFloat4(uDiffuseColor, &diffuseColor.r);
    }

    void DiffuseMaterial::SetDiffuseTexture(Texture2D *texture)
    {
        this->pDiffuseTexture = texture;
    }

    Texture2D *DiffuseMaterial::GetDiffuseTexture() const
    {
        return pDiffuseTexture;
    }

    void DiffuseMaterial::SetDiffuseColor(const Color &color)
    {
        this->diffuseColor = color;
    }

    Color DiffuseMaterial::GetDiffuseColor() const
    {
        return diffuseColor;
    }
};