#include "ParticleMaterial.hpp"
#include "../../Core/Resources.hpp"

namespace GravyEngine
{
    ParticleMaterial::ParticleMaterial() : Material()
    {
        pDiffuseTexture = nullptr;
        uvOffset = Vector2(0, 0);
        uvScale = Vector2(1, 1);
        alphaCutOff = 0.0f;
        blendMode = ParticleBlendMode::Add;

        pShader = Resources::FindShader("Particle");

        if(pShader)
        {
            uTexture = glGetUniformLocation(pShader->GetId(), "uTexture");
            uUVOffset = glGetUniformLocation(pShader->GetId(), "uUVOffset");
            uUVScale = glGetUniformLocation(pShader->GetId(), "uUVScale");
            uBlendMode = glGetUniformLocation(pShader->GetId(), "uBlendMode");
            uAlphaCutOff = glGetUniformLocation(pShader->GetId(), "uAlphaCutOff");
        }

        SetName("ParticleMaterial");
    }
    
    void ParticleMaterial::Use(Transform *transform, Camera *camera)
    {
        if(!pShader || !transform || !camera)
            return;

        pShader->Use();

        if(pDiffuseTexture != nullptr)
        {
            pDiffuseTexture->Bind(0);
            pShader->SetInt(uTexture, 0);
        }

        pShader->SetFloat2(uUVOffset, glm::value_ptr(uvOffset));
        pShader->SetFloat2(uUVScale, glm::value_ptr(uvScale));
        pShader->SetFloat(uAlphaCutOff, alphaCutOff);
        pShader->SetFloat(uBlendMode, blendMode == ParticleBlendMode::Add ? -1.0f : 1.0f);
    }

    void ParticleMaterial::SetDiffuseTexture(Texture2D *texture)
    {
        this->pDiffuseTexture = texture;
    }

    Texture2D *ParticleMaterial::GetDiffuseTexture() const
    {
        return pDiffuseTexture;
    }

    void ParticleMaterial::SetUVOffset(const Vector2 &uvOffset)
    {
        this->uvOffset = uvOffset;
    }

    Vector2 ParticleMaterial::GetUVOffset() const
    {
        return uvOffset;
    }

    void ParticleMaterial::SetUVScale(const Vector2 &uvScale)
    {
        this->uvScale = uvScale;
    }

    Vector2 ParticleMaterial::GetUVScale() const
    {
        return uvScale;
    }

    void ParticleMaterial::SetAlphaCutOff(float alphaCutOff)
    {
        this->alphaCutOff = alphaCutOff;
    }

    float ParticleMaterial::GetAlphaCutOff() const
    {
        return alphaCutOff;
    }

    void ParticleMaterial::SetBlendMode(ParticleBlendMode blendMode)
    {
        this->blendMode = blendMode;
    }

    ParticleBlendMode ParticleMaterial::GetBlendMode() const
    {
        return blendMode;
    }
};