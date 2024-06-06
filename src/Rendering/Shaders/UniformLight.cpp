#include "UniformLight.hpp"

namespace GravyEngine
{
    static std::string source = R"(struct LightInfo
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
} Lights;)";

    std::string UniformLight::GetSource()
    {
        return source;
    }
};