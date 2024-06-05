#include "ShaderCore.hpp"

namespace GravyEngine
{
    static std::string source = R"(const float PI = 3.14159265359;

vec4 GetWorldPosition(mat4 projection, mat4 view, mat4 model, vec3 position)
{
    return model * view * projection * vec4(position, 1.0);
}

vec4 GammaCorrection(vec3 color)
{
    return vec4(pow(color, vec3(1.0/2.2)), 1.0);
}

float InverseLerp(float start, float end, float value)
{
    return (value - start) / (end - start);
}

float GetFogVisibility(float density, float gradient, vec3 camPosition, vec3 fragPosition)
{
    float fogDistance = length(camPosition - fragPosition);
    //float fogVisibility = exp(-pow(fogDistance * density, gradient));
    float d = (fogDistance * density) * (fogDistance * density);
    float fogVisibility = pow(2, -d);

    fogVisibility = clamp(fogVisibility, 0.0f, 1.0f);
    return fogVisibility;
})";

    std::string ShaderCore::GetSource()
    {
        return source;
    }
};