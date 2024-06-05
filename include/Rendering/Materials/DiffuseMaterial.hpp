#ifndef DIFFUSEMATERIAL_HPP
#define DIFFUSEMATERIAL_HPP

#include "../Material.hpp"
#include "../Texture2D.hpp"

namespace GravyEngine
{
    class DiffuseMaterial : public Material
    {
    public:
        DiffuseMaterial();
        void Use(Transform *transform, Camera *camera) override;
        void SetDiffuseTexture(Texture2D *texture);
        Texture2D *GetDiffuseTexture() const;
    private:
        int uModel;
        int uModelInverted;
        int uMVP;
        int uDiffuseTexture;
        int uUVOffset;
        int uUVScale;
        Texture2D *pDiffuseTexture;
    };
};

#endif