#ifndef DIFFUSEMATERIAL_HPP
#define DIFFUSEMATERIAL_HPP

#include "../Material.hpp"

namespace GravyEngine
{
    class DiffuseMaterial : public Material
    {
    public:
        DiffuseMaterial();
        void UpdateUniforms() override;
    private:
        int uModel;
        int uModelInverted;
        int uMVP;
        int uDiffuseTexture;
        int uUVOffset;
        int uUVScale;
    };
};

#endif