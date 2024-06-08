#ifndef GRAVYENGINE_DIFFUSEMATERIAL_HPP
#define GRAVYENGINE_DIFFUSEMATERIAL_HPP

#include "../Material.hpp"
#include "../Texture2D.hpp"
#include "../Texture2DArray.hpp"
#include "../../System/Drawing/Color.hpp"
#include "../../System/Numerics/Vector2.hpp"

namespace GravyEngine
{
    class DiffuseMaterial : public Material
    {
    public:
        DiffuseMaterial();
        void Use(Transform *transform, Camera *camera) override;
        void SetDiffuseTexture(Texture2D *texture);
        Texture2D *GetDiffuseTexture() const;
        void SetDiffuseColor(const Color &color);
        Color GetDiffuseColor() const;
        void SetAmbientStrength(float ambientStrength);
        float GetAmbientStrength() const;
        void SetShininess(float shininess);
        float GetShininess() const;
        void SetUVOffset(const Vector2 &uvOffset);
        Vector2 GetUVOffset() const;
        void SetUVScale(const Vector2 &uvScale);
        Vector2 GetUVScale() const;
        void SetReceiveShadows(bool receiveShadows);
        bool GetReceiveShadows() const;
    private:
        int uModel;
        int uModelInverted;
        int uMVP;
        int uDiffuseTexture;
        int uDiffuseColor;
        int uAmbientStrength;
        int uShininess;
        int uUVOffset;
        int uUVScale;
        int uDepthMap;
        int uReceiveShadows;
        Texture2D *pDiffuseTexture;
        Color diffuseColor;
        float ambientStrength;
        float shininess;
        Vector2 uvOffset;
        Vector2 uvScale;
        Texture2DArray *pDepthMap;
        bool receiveShadows;
    };
};

#endif