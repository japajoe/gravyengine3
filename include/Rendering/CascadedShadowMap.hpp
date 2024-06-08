#ifndef GRAVYENGINE_CASCADEDSHADOWMAP_HPP
#define GRAVYENGINE_CASCADEDSHADOWMAP_HPP

#include "Texture2DArray.hpp"
#include "Shader.hpp"
#include "Buffers/UniformBufferObject.hpp"
#include "../Core/Camera.hpp"
#include "../Core/Light.hpp"
#include "../System/Numerics/Vector2.hpp"
#include "../System/Numerics/Vector3.hpp"
#include "../System/Numerics/Vector4.hpp"
#include "../System/Numerics/Matrix4.hpp"
#include <vector>
#include <memory>

namespace GravyEngine
{
    struct UniformShadowInfo
    {
        int cascadeCount;
        float shadowBias;
        float farPlane;
        float padding;
        Matrix4 lightSpaceMatrices[16];
        //float cascadePlaneDistances[16];
        Vector4 cascadePlaneDistances[16];
    };

    class CascadedShadowMap
    {
    private:
        unsigned int lightFBO;
        UniformBufferObject *matricesUBO;
        Texture2DArray *depthMap;
        Shader *shader;
        Camera *camera;
        Light *light;
        UniformShadowInfo shadowData;
        std::vector<float> shadowCascadeLevels;
        std::vector<Vector4> GetFrustumCornersWorldSpace(const Matrix4 &projview);
        std::vector<Vector4> GetFrustumCornersWorldSpace(const Matrix4 &proj, const Matrix4 &view);
        Matrix4 GetLightSpaceMatrix(const float nearPlane, const float farPlane);
        std::vector<Matrix4> GetLightSpaceMatrices();
    public:
        CascadedShadowMap();
        CascadedShadowMap(Texture2DArray *depthMap, UniformBufferObject *matricesUBO, Camera *camera, Light *light);
        void SetCamera(Camera *camera);
        void SetLight(Light *light);
        void Bind();
        void Unbind();
        void Delete();
        std::vector<float> &GetShadowCascadeLevels();
    };
};

#endif