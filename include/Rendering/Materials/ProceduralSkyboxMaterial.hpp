#ifndef GRAVYENGINE_PROCEDURALSKYBOXMATERIAL_HPP
#define GRAVYENGINE_PROCEDURALSKYBOXMATERIAL_HPP

#include "../Material.hpp"
#include "../../System/Numerics/Vector3.hpp"

namespace GravyEngine
{
    class ProceduralSkyboxMaterial : public Material
    {
    public:
        ProceduralSkyboxMaterial();
        void Use(Transform *transform, Camera *camera) override;
    private:
        int uModel;
        int uView;
        int uProjection;
        int uRayleighCoefficient;
        int uMieCoefficient;
        int uScatteringDirection;
        int uCloudSpeed;
        int uCirrus;
        int uCumulus;
        int uSunPosition;
        int uFogStart;
        int uFogEnd;

        float cloudSpeed;
        float cirrus;
        float cumulus;
        float rayleighCoefficient;
        float mieCoefficient;
        float scatteringDirection;
        Vector3 sunPosition;
        float fogStart;
        float fogEnd;

        void SetCloudSpeed(float speed);
        float GetCloudSpeed() const;
        void SetCirrus(float cirrus);
        float GetCirrus() const;
        void SetCumulus(float cumulus);
        float GetCumulus() const;
        void SetRayleighCoefficient(float rayleighCoefficient);
        float GetRayleighCoefficient() const;
        void SetMieCoefficient(float mieCoefficient);
        float GetMieCoefficient() const;
        void SetScatteringDirection(float scatteringDirection);
        float GetScatteringDirection() const;
        void SetSunPosition(const Vector3 &sunPosition);
        Vector3 GetSunPosition() const;
        void SetFogStart(float value);
        float GetFogStart() const;
        void SetFogEnd(float value);
        float GetFogEnd() const;
    };
};

#endif