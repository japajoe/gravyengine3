#ifndef GRAVYENGINE_DIFFUSEMATERIAL_HPP
#define GRAVYENGINE_DIFFUSEMATERIAL_HPP

#include "../Material.hpp"
#include "../Texture2D.hpp"
#include "../../System/Drawing/Color.hpp"

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
    private:
        int uModel;
        int uModelInverted;
        int uMVP;
        int uDiffuseTexture;
        int uDiffuseColor;
        int uUVOffset;
        int uUVScale;
        Texture2D *pDiffuseTexture;
        Color diffuseColor;
    };
};

#endif