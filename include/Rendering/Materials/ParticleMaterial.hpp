#ifndef GRAVYENGINE_PARTICLEMATERIAL_HPP
#define GRAVYENGINE_PARTICLEMATERIAL_HPP

#include "../Material.hpp"
#include "../Texture2D.hpp"
#include "../../System/Drawing/Color.hpp"
#include "../../System/Numerics/Vector2.hpp"

namespace GravyEngine
{
    enum class ParticleBlendMode
    {
        Add,
        Multiply
    };

    class ParticleMaterial : public Material
    {
    public:
        ParticleMaterial();
        void Use(Transform *transform, Camera *camera) override;
        void SetDiffuseTexture(Texture2D *texture);
        Texture2D *GetDiffuseTexture() const;
        void SetUVOffset(const Vector2 &uvOffset);
        Vector2 GetUVOffset() const;
        void SetUVScale(const Vector2 &uvScale);
        Vector2 GetUVScale() const;
        void SetAlphaCutOff(float alphaCutOff);
        float GetAlphaCutOff() const;
        void SetBlendMode(ParticleBlendMode blendMode);
        ParticleBlendMode GetBlendMode() const;
    private:
        int uTexture;
        int uUVOffset;
        int uUVScale;        
        int uBlendMode;
        int uAlphaCutOff;
        Texture2D *pDiffuseTexture;
        Vector2 uvOffset;
        Vector2 uvScale;
        float alphaCutOff;
        ParticleBlendMode blendMode;
    };
};

#endif