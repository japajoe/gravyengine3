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
        ambientStrength = 0.5f;
        shininess = 16.0f;
        uvOffset = Vector2(0, 0);
        uvScale = Vector2(1, 1);

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
            uAmbientStrength = glGetUniformLocation(pShader->GetId(), "uAmbientStrength");
            uShininess = glGetUniformLocation(pShader->GetId(), "uShininess");
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

        pShader->Use();

        pShader->SetMat4(uModel, glm::value_ptr(model));
        pShader->SetMat4(uModelInverted, glm::value_ptr(modelInverted));
        pShader->SetMat4(uMVP, glm::value_ptr(MVP));
        pShader->SetFloat4(uDiffuseColor, &diffuseColor.r);
        pShader->SetFloat(uAmbientStrength, ambientStrength);
        pShader->SetFloat(uShininess, shininess);
        pShader->SetFloat2(uUVOffset, glm::value_ptr(uvOffset));
        pShader->SetFloat2(uUVScale, glm::value_ptr(uvScale));

        if(pDiffuseTexture != nullptr)
        {
            uint32_t unit = 0;
            pDiffuseTexture->Bind(unit);
            pShader->SetInt(uDiffuseTexture, unit);
            unit++;
        }
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

    void DiffuseMaterial::SetAmbientStrength(float ambientStrength)
    {
        this->ambientStrength = ambientStrength;
    }

    float DiffuseMaterial::GetAmbientStrength() const
    {
        return ambientStrength;
    }

    void DiffuseMaterial::SetShininess(float shininess)
    {
        this->shininess = shininess;
    }

    float DiffuseMaterial::GetShininess() const
    {
        return shininess;
    }

    void DiffuseMaterial::SetUVOffset(const Vector2 &uvOffset)
    {
        this->uvOffset = uvOffset;
    }

    Vector2 DiffuseMaterial::GetUVOffset() const
    {
        return uvOffset;
    }

    void DiffuseMaterial::SetUVScale(const Vector2 &uvScale)
    {
        this->uvScale = uvScale;
    }

    Vector2 DiffuseMaterial::GetUVScale() const
    {
        return uvScale;
    }
};