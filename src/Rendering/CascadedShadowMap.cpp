#include "CascadedShadowMap.hpp"
#include "../Core/Debug.hpp"
#include "../Core/Screen.hpp"
#include "../Core/WorldSettings.hpp"
#include "../Core/Resources.hpp"
#include "../External/glad/glad.h"

namespace GravyEngine
{
    CascadedShadowMap::CascadedShadowMap()
    {
        shader = nullptr;
        camera = nullptr;
        light = nullptr;
        lightFBO = 0;
        matricesUBO = nullptr;
        depthMap = nullptr;
    }

    CascadedShadowMap::CascadedShadowMap(Texture2DArray *depthMap, UniformBufferObject *matricesUBO, Camera *camera, Light *light)
    {
        this->depthMap = depthMap;
        shader = Resources::FindShader("Depth");

        this->camera = camera;
        this->light = light;
        float farPlane = camera->GetFarClippingPlane();
        
        shadowCascadeLevels = 
        { 
            farPlane / 50.0f, farPlane / 25.0f, farPlane / 10.0f, farPlane / 2.0f 
        };

        shadowData.cascadeCount = shadowCascadeLevels.size();

        lightFBO = 0;
        this->matricesUBO = matricesUBO;

        // configure light FBO
        // -----------------------
        glGenFramebuffers(1, &lightFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap->GetId(), 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            Debug::WriteError("CascadedShadowMap framebuffer is not complete");
            throw 0;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void CascadedShadowMap::SetCamera(Camera *camera)
    {
        this->camera = camera;
    }

    void CascadedShadowMap::SetLight(Light *light)
    {
        this->light = light;
    }

    void CascadedShadowMap::Bind()
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 0. UBO setup
        const auto lightMatrices = GetLightSpaceMatrices();
        for(size_t i = 0; i < lightMatrices.size(); i++)
        {
            shadowData.lightSpaceMatrices[i] = lightMatrices[i];
        }

        //Bullshit hack because OpenGL adds 16 bytes padding for each float in an array
        size_t index = 0;
        for(size_t i = 0; i < shadowCascadeLevels.size(); i++)
        {
            shadowData.cascadePlaneDistances[i].x = shadowCascadeLevels[i];
            // shadowData.cascadePlaneDistances[index+0] = shadowCascadeLevels[i];
            // shadowData.cascadePlaneDistances[index+1] = shadowCascadeLevels[i];
            // shadowData.cascadePlaneDistances[index+2] = shadowCascadeLevels[i];
            // shadowData.cascadePlaneDistances[index+3] = shadowCascadeLevels[i];
            // index+=4;
        }
        
        shadowData.farPlane = camera->GetFarClippingPlane();
        shadowData.shadowBias = WorldSettings::GetShadowBias();

        matricesUBO->Bind();
        matricesUBO->BufferSubData(0, sizeof(UniformShadowInfo), &shadowData);
        matricesUBO->Unbind();

        glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
        glViewport(0, 0, depthMap->GetWidth(), depthMap->GetHeight());
        glClear(GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_FRONT);  // peter panning
    }

    void CascadedShadowMap::Unbind()
    {
        Vector4 viewportRect = Screen::GetViewportRect();
        glCullFace(GL_BACK);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // reset viewport
        glViewport(0, 0, (int)viewportRect.z, (int)viewportRect.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    //The projection matrix described here is a perspective matrix, using the camera’s aspect ratio and fov, and using the near and far plane of the current frustum being analyzed. The view matrix is the view matrix of our camera.
    // const auto proj = glm::perspective(
    //     glm::radians(camera.Zoom),
    //     (float)SCR_WIDTH / (float)SCR_HEIGHT,
    //     nearPlane,
    //     farPlane
    // );
    std::vector<Vector4> CascadedShadowMap::GetFrustumCornersWorldSpace(const Matrix4 &projview)
    {
        const auto inv = Matrix4f::Invert(projview);

        std::vector<Vector4> frustumCorners;
        for (unsigned int x = 0; x < 2; ++x)
        {
            for (unsigned int y = 0; y < 2; ++y)
            {
                for (unsigned int z = 0; z < 2; ++z)
                {
                    const Vector4 pt = inv * Vector4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
                    frustumCorners.push_back(pt / pt.w);
                }
            }
        }

        return frustumCorners;
    }

    std::vector<Vector4> CascadedShadowMap::GetFrustumCornersWorldSpace(const Matrix4 &proj, const Matrix4 &view)
    {
        return GetFrustumCornersWorldSpace(proj * view);
    }

    Matrix4 CascadedShadowMap::GetLightSpaceMatrix(const float nearPlane, const float farPlane)
    {
        Vector4 viewportRect = Screen::GetViewportRect();

        const auto proj = glm::perspective(
            glm::radians(camera->GetFieldOfView()), viewportRect.z / viewportRect.w, nearPlane,
            farPlane);
        
        const auto corners = GetFrustumCornersWorldSpace(proj, camera->GetViewMatrix());

        glm::vec3 center = glm::vec3(0, 0, 0);
        for (const auto& v : corners)
        {
            center += glm::vec3(v);
        }
        center /= corners.size();

        Vector3 lightDir = light->GetTransform()->GetForward();

        const auto lightView = glm::lookAt(center + lightDir, center, Vector3(0.0f, 1.0f, 0.0f));

        float minX = std::numeric_limits<float>::max();
        float maxX = std::numeric_limits<float>::lowest();
        float minY = std::numeric_limits<float>::max();
        float maxY = std::numeric_limits<float>::lowest();
        float minZ = std::numeric_limits<float>::max();
        float maxZ = std::numeric_limits<float>::lowest();
        for (const auto& v : corners)
        {
            const auto trf = lightView * v;
            minX = std::min(minX, trf.x);
            maxX = std::max(maxX, trf.x);
            minY = std::min(minY, trf.y);
            maxY = std::max(maxY, trf.y);
            minZ = std::min(minZ, trf.z);
            maxZ = std::max(maxZ, trf.z);
        }

        // Tune this parameter according to the scene
        constexpr float zMult = 1.0f;
        if (minZ < 0)
        {
            minZ *= zMult;
        }
        else
        {
            minZ /= zMult;
        }
        if (maxZ < 0)
        {
            maxZ /= zMult;
        }
        else
        {
            maxZ *= zMult;
        }

        const Matrix4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
        return lightProjection * lightView;
    }

    std::vector<Matrix4> CascadedShadowMap::GetLightSpaceMatrices()
    {
        float cameraNearPlane = camera->GetNearClippingPlane();
        float cameraFarPlane = camera->GetFarClippingPlane();

        std::vector<Matrix4> ret;
        for (size_t i = 0; i < shadowCascadeLevels.size() + 1; ++i)
        {
            if (i == 0)
            {
                ret.push_back(GetLightSpaceMatrix(cameraNearPlane, shadowCascadeLevels[i]));
            }
            else if (i < shadowCascadeLevels.size())
            {
                ret.push_back(GetLightSpaceMatrix(shadowCascadeLevels[i - 1], shadowCascadeLevels[i]));
            }
            else
            {
                ret.push_back(GetLightSpaceMatrix(shadowCascadeLevels[i - 1], cameraFarPlane));
            }
        }
        return ret;
    }

    void CascadedShadowMap::Delete()
    {
        if(lightFBO > 0)
        {
            glDeleteFramebuffers(1, &lightFBO);
            lightFBO = 0;
        }
    }

    std::vector<float> &CascadedShadowMap::GetShadowCascadeLevels()
    {
        return shadowCascadeLevels;
    }
};