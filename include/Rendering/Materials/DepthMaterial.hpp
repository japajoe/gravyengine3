#ifndef GRAVYENGINE_DEPTHMATERIAL_HPP
#define GRAVYENGINE_DEPTHMATERIAL_HPP

#include "../Material.hpp"

namespace GravyEngine
{
    class DepthMaterial : public Material
    {
    public:
        DepthMaterial();
        void Use(Transform *transform, Camera *camera) override;
    private:
        int uModel;
    };
};

#endif