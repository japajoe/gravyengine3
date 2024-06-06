#include "ProceduralSkyboxMaterial.hpp"
#include "../../Core/Time.hpp"
#include "../../Core/WorldSettings.hpp"
#include "../../System/Numerics/Matrix3.hpp"
#include "../../System/Numerics/Matrix4.hpp"
#include "../../System/Numerics/Vector2.hpp"
#include "../../External/glad/glad.h"

namespace GravyEngine
{
    ProceduralSkyboxMaterial::ProceduralSkyboxMaterial() : Material()
    {
        pShader = Shader::Find("ProceduralSkybox");

        if(pShader)
        {
            uModel = glGetUniformLocation(pShader->GetId(), "uModel");
            uView = glGetUniformLocation(pShader->GetId(), "uView");
            uProjection = glGetUniformLocation(pShader->GetId(), "uProjection");
            uRayleighCoefficient = glGetUniformLocation(pShader->GetId(), "uRayleighCoefficient");
            uMieCoefficient = glGetUniformLocation(pShader->GetId(), "uMieCoefficient");
            uScatteringDirection = glGetUniformLocation(pShader->GetId(), "uScatteringDirection");
            uTime = glGetUniformLocation(pShader->GetId(), "uTime");
            uCloudSpeed = glGetUniformLocation(pShader->GetId(), "uCloudSpeed");
            uCirrus = glGetUniformLocation(pShader->GetId(), "uCirrus");
            uCumulus = glGetUniformLocation(pShader->GetId(), "uCumulus");
            uSunPosition = glGetUniformLocation(pShader->GetId(), "uSunPosition");
            uRenderFog = glGetUniformLocation(pShader->GetId(), "uRenderFog");
            uFogStart = glGetUniformLocation(pShader->GetId(), "uFogStart");
            uFogEnd = glGetUniformLocation(pShader->GetId(), "uFogEnd");
            uFogColor = glGetUniformLocation(pShader->GetId(), "uFogColor");
        }

        cloudSpeed = 0.1f;
        cirrus = 0.677f;
        cumulus = 0.403f;
        rayleighCoefficient = 0.0075f;
        mieCoefficient = 0.0153f;
        scatteringDirection = 0.998f; // Mie scattering direction. Should be ALMOST 1.0f
        sunPosition = Vector3(-0.007f, 0.954f, -1.563f);
        fogStart = 0.0f;
        fogEnd = 0.2f;

        SetName("ProceduralSkyboxMaterial");
    }

    void ProceduralSkyboxMaterial::Use(Transform *transform, Camera *camera)
    {
        if(!pShader)
            return;
        if(!camera)
            return;
        if(!transform)
            return;

        Matrix4 projection = camera->GetProjectionMatrix();
        Matrix4 view = camera->GetViewMatrix();
        Matrix4 model = transform->GetModelMatrix();
        Color fogColor = WorldSettings::GetFogColor();

        pShader->Use();

        pShader->SetMat4(uModel, glm::value_ptr(model));
        pShader->SetMat4(uView, glm::value_ptr(view));
        pShader->SetMat4(uProjection, glm::value_ptr(projection));

        pShader->SetFloat(uTime, Time::GetTime());
        pShader->SetFloat(uCloudSpeed, cloudSpeed);
        pShader->SetFloat(uCirrus, cirrus);
        pShader->SetFloat(uCumulus, cumulus);
        pShader->SetFloat(uRayleighCoefficient, rayleighCoefficient);
        pShader->SetFloat(uMieCoefficient, mieCoefficient);
        pShader->SetFloat(uScatteringDirection, scatteringDirection);
        pShader->SetFloat3(uSunPosition, &sunPosition.x);
        pShader->SetFloat(uRenderFog, WorldSettings::GetFogEnabled() ? 1.0f : -1.0f);
        pShader->SetFloat(uFogStart, fogStart);
        pShader->SetFloat(uFogEnd, fogEnd);
        pShader->SetFloat3(uFogColor, &fogColor.r);
    }

    void ProceduralSkyboxMaterial::SetCloudSpeed(float speed)
    {
        this->cloudSpeed = speed;
    }

    float ProceduralSkyboxMaterial::GetCloudSpeed() const
    {
        return cloudSpeed;
    }

    void ProceduralSkyboxMaterial::SetCirrus(float cirrus)
    {
        this->cirrus = cirrus;
    }

    float ProceduralSkyboxMaterial::GetCirrus() const
    {
        return cirrus;
    }

    void ProceduralSkyboxMaterial::SetCumulus(float cumulus)
    {
        this->cumulus = cumulus;
    }

    float ProceduralSkyboxMaterial::GetCumulus() const
    {
        return cumulus;
    }

    void ProceduralSkyboxMaterial::SetRayleighCoefficient(float rayleighCoefficient)
    {
        this->rayleighCoefficient = rayleighCoefficient;
    }

    float ProceduralSkyboxMaterial::GetRayleighCoefficient() const
    {
        return rayleighCoefficient;
    }

    void ProceduralSkyboxMaterial::SetMieCoefficient(float mieCoefficient)
    {
        this->mieCoefficient = mieCoefficient;
    }

    float ProceduralSkyboxMaterial::GetMieCoefficient() const
    {
        return mieCoefficient;
    }

    void ProceduralSkyboxMaterial::SetScatteringDirection(float scatteringDirection)
    {
        this->scatteringDirection = scatteringDirection;
    }

    float ProceduralSkyboxMaterial::GetScatteringDirection() const
    {
        return scatteringDirection;
    }

    void ProceduralSkyboxMaterial::SetSunPosition(const Vector3 &sunPosition)
    {
        this->sunPosition = sunPosition;
    }

    Vector3 ProceduralSkyboxMaterial::GetSunPosition() const
    {
        return sunPosition;
    }

    void ProceduralSkyboxMaterial::SetFogStart(float value)
    {
        fogStart = value;
    }

    float ProceduralSkyboxMaterial::GetFogStart() const
    {
        return fogStart;
    }

    void ProceduralSkyboxMaterial::SetFogEnd(float value)
    {
        fogEnd = value;
    }

    float ProceduralSkyboxMaterial::GetFogEnd() const
    {
        return fogEnd;
    }
};