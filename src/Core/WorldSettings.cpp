#include "WorldSettings.hpp"

namespace GravyEngine
{
    Color WorldSettings::fogColor = Color(247, 169, 90, 255);
    float WorldSettings::fogDensity = 0.001f;
    float WorldSettings::fogGradient = 1.5f;
    bool WorldSettings::fogEnabled = true;
    float WorldSettings::shadowBias = 0.005f;

    void WorldSettings::SetFogEnabled(bool enabled)
    {
        fogEnabled = enabled;
    }

    bool WorldSettings::GetFogEnabled()
    {
        return fogEnabled;
    }

    void WorldSettings::SetFogColor(const Color &value)
    {
        fogColor = value;
    }

    Color WorldSettings::GetFogColor()
    {
        return fogColor;
    }

    void WorldSettings::SetFogGradient(float value)
    {
        fogGradient = value;
    }

    float WorldSettings::GetFogGradient()
    {
        return fogGradient;
    }

    void WorldSettings::SetFogDensity(float value)
    {
        fogDensity = value;
    }

    float WorldSettings::GetFogDensity()
    {
        return fogDensity;
    }

    void WorldSettings::SetShadowBias(float bias)
    {
        shadowBias = bias;
    }

    float WorldSettings::GetShadowBias()
    {
        return shadowBias;
    }
};