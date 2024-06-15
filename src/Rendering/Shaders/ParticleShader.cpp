#include "ParticleShader.hpp"

namespace GravyEngine
{
    static std::string vertex = R"(#version 420 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in mat4 aInstanceModel;
layout (location = 7) in vec4 aInstanceColor;

#include <ShaderCore>

out vec3 oFragPosition;
out vec3 oNormal;
out vec2 oUV;
out vec4 oColor;

void main()
{
    oFragPosition = vec3(aInstanceModel * vec4(aPosition, 1.0)); // Vertex in world space
    oNormal = inverse(transpose(mat3(aInstanceModel))) * aNormal;
    oUV = aUV;
    oColor = aInstanceColor;
    gl_Position = Camera.viewProjection * aInstanceModel * vec4(aPosition, 1.0);
})";

    static std::string fragment = R"(#version 420 core
#include <ShaderCore>

uniform sampler2D uTexture;
uniform vec2 uUVOffset;
uniform vec2 uUVScale;
uniform float uBlendMode;
uniform float uAlphaCutOff;
uniform vec3 uFogColor;
uniform float uFogDensity;
uniform float uFogGradient;

in vec3 oFragPosition;
in vec3 oNormal;
in vec2 oUV;
in vec4 oColor;

out vec4 FragColor;

void main()
{
    vec4 finalColor = oColor * texture2D(uTexture, (oUV + uUVOffset) * uUVScale);

    if(finalColor.a < uAlphaCutOff)
        discard;

    float alpha = finalColor.a;

    // if(FogSettings.enabled > 0)
    // {
    //     float visibility = GetFogVisibility(FogSettings.density, FogSettings.gradient, Camera.position.xyz, oFragPosition);
    //     finalColor.rgb = mix(FogSettings.color.rgb, finalColor.rgb, visibility);
    // }

    finalColor = GammaCorrection(finalColor.xyz);
    finalColor.a = alpha;

    FragColor = finalColor;
})";

    static const char *name = "Particle";

    Shader *ParticleShader::Create()
    {
        if(Shader::Find(name) != nullptr)
            return nullptr;

        Shader shader(vertex, fragment);
        return Shader::Add(name, shader);
    }

    void ParticleShader::Destroy()
    {
        Shader::Remove(name);
    }
};