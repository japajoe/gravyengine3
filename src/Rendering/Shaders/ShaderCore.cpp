#include "ShaderCore.hpp"

namespace GravyEngine
{
    static std::string source = 
R"(layout(std140, binding = 0) uniform uCamera
{
    mat4 view;
    mat4 projection;
    mat4 viewProjection;
    vec4 position;
} Camera;

struct LightInfo
{
    int isActive;       //4
    int type;           //4
    float constant;     //4
    float linear;       //4
    float quadratic;    //4
    float strength;     //4
    float padding1;     //4
    float padding2;     //4
    vec4 position;      //16
    vec4 direction;     //16
    vec4 color;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

#define MAX_NUM_LIGHTS 32

layout(std140, binding = 1) uniform uLights
{
    LightInfo lights[MAX_NUM_LIGHTS];
} Lights;

uniform sampler2DArray uDepthMap;

layout (std140, binding = 2) uniform uShadow
{
    int cascadeCount;
    float shadowBias;
    float farPlane;
    float padding;
    mat4 lightSpaceMatrices[16];
    float cascadePlaneDistances[16];
} Shadow;

layout(std140, binding = 3) uniform uWorld
{
    vec4 fogColor;      //16 don't use vec3 because the alignment causes issues
    float fogDensity;   //4
    float fogGradient;  //4
    int fogEnabled;     //4
    float time;         //4
} World;

const float PI = 3.14159265359;

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
}

float ShadowCalculation(vec3 fragPosWorldSpace, mat4 view, vec3 normal, vec3 lightDirection)
{
    // select cascade layer
    vec4 fragPosViewSpace = view * vec4(fragPosWorldSpace, 1.0);
    float depthValue = abs(fragPosViewSpace.z);

    int layer = -1;
    for (int i = 0; i < Shadow.cascadeCount; ++i)
    {
        if (depthValue < Shadow.cascadePlaneDistances[i])
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
    {
        layer = Shadow.cascadeCount;
    }

    vec4 fragPosLightSpace = Shadow.lightSpaceMatrices[layer] * vec4(fragPosWorldSpace, 1.0);
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (currentDepth > 1.0)
    {
        return 0.0;
    }
    // calculate bias (based on depth map resolution and slope)
    normal = normalize(normal);
    float maxBias = Shadow.shadowBias; //default 0.005f;
    float bias = max(0.05 * (1.0 - dot(normal, lightDirection)), maxBias);
    const float biasModifier = 0.5f;
    if (layer == Shadow.cascadeCount)
    {
        bias *= 1 / (Shadow.farPlane * biasModifier);
    }
    else
    {
        bias *= 1 / (Shadow.cascadePlaneDistances[layer] * biasModifier);
    }

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(uDepthMap, 0));
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(uDepthMap, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r;
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
        
    return shadow;
})";

    std::string ShaderCore::GetSource()
    {
        return source;
    }
};