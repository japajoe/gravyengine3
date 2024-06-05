#ifndef GRAVYENGINE_WORLDSETTINGS_HPP
#define GRAVYENGINE_WORLDSETTINGS_HPP

#include "../System/Drawing/Color.hpp"

namespace GravyEngine
{
    struct FogSettingsData
    {
        float color[4];
        float density;
        float gradient;
        int enabled;
    };

    class WorldSettings
    {
    private:
        static Color fogColor;
        static float fogDensity;
        static float fogGradient;
        static bool fogEnabled;
        static float shadowBias;
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
    };
};

#endif