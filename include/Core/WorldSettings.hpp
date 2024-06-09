#ifndef GRAVYENGINE_WORLDSETTINGS_HPP
#define GRAVYENGINE_WORLDSETTINGS_HPP

#include "../System/Drawing/Color.hpp"

namespace GravyEngine
{
    struct UniformWorldInfo
    {
        Color fogColor;
        float fogDensity;
        float fogGradient;
        int fogEnabled;
        float time;
    };

    class WorldSettings
    {
    public:
        static void SetFogEnabled(bool enabled);
        static bool GetFogEnabled();
        static void SetFogColor(const Color &value);
        static Color GetFogColor();
        static void SetFogGradient(float value);
        static float GetFogGradient();
        static void SetFogDensity(float value);
        static float GetFogDensity();
        static void SetShadowBias(float bias);
        static float GetShadowBias();
        static void UpdateUniformBuffer();
    private:
        static Color fogColor;
        static float fogDensity;
        static float fogGradient;
        static bool fogEnabled;
        static float shadowBias;
    };
};

#endif