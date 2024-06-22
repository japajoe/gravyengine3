#include "DepthMaterial.hpp"
#include "../../Core/Resources.hpp"
#include "../../System/Numerics/Matrix4.hpp"
#include "../../External/glad/glad.h"

namespace GravyEngine
{
    DepthMaterial::DepthMaterial() : Material()
    {
        pShader = Resources::FindShader("Depth");

        if(pShader)
        {
            uModel = glGetUniformLocation(pShader->GetId(), "uModel");
        }

        SetName("DepthMaterial");
    }

    void DepthMaterial::Use(Transform *transform, Camera *camera)
    {
        if(!pShader || !transform || !camera)
            return;

        Matrix4 model = transform->GetModelMatrix();
        pShader->Use();
        pShader->SetMat4(uModel, glm::value_ptr(model));
    }
};