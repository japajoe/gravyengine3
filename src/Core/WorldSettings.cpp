#include "WorldSettings.hpp"
#include "Time.hpp"
#include "../Rendering/Buffers/UniformBufferObject.hpp"

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

    static UniformBufferObject *uniformBuffer = nullptr;

    void WorldSettings::UpdateUniformBuffer()
    {
        if(uniformBuffer == nullptr)
        {
            uniformBuffer = UniformBufferObject::Find("World");

            if(uniformBuffer == nullptr)
                return;
        }
        
        UniformWorldInfo worldInfo;
        worldInfo.fogColor = fogColor;
        worldInfo.fogDensity = fogDensity;
        worldInfo.fogGradient = fogGradient;
        worldInfo.fogEnabled = fogEnabled ? 1 : -1;
        worldInfo.time = Time::GetTime();

        uniformBuffer->Bind();
        uniformBuffer->BufferSubData(0, sizeof(UniformWorldInfo), &worldInfo);
        uniformBuffer->Unbind();
    }
};