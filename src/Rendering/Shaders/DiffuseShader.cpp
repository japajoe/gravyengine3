#include "DiffuseShader.hpp"

namespace GravyEngine
{
    static std::string vertex = 
R"(#version 420 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

uniform mat4 uModel;
uniform mat3 uModelInverted;
uniform mat4 uMVP;

out vec3 oNormal;
out vec3 oFragPosition;
out vec2 oUV;

void main() {
    gl_Position = uMVP * vec4(aPosition, 1.0);
    oNormal = uModelInverted * aNormal;
    oFragPosition = vec3(uModel * vec4(aPosition, 1.0));
    oUV = aUV;
})";

    static std::string fragment = 
R"(#version 420 core
#include <ShaderCore>

uniform sampler2D uDiffuseTexture;
uniform vec4 uDiffuseColor;
uniform float uAmbientStrength;
uniform float uShininess;
uniform vec2 uUVOffset;
uniform vec2 uUVScale;
uniform int uReceiveShadows;

in vec3 oNormal;
in vec3 oFragPosition;
in vec2 oUV;

out vec4 FragColor;

void main() {
    vec4 texColor = texture(uDiffuseTexture, (oUV + uUVOffset) * uUVScale);
    vec3 normal = normalize(oNormal);

    vec3 lightDirection = normalize(Lights.lights[0].direction.xyz);
    //vec3 lightDirection = normalize(Lights.lights[0].position.xyz - oFragPosition);

    float shadow  = 0.0;
    if(uReceiveShadows > 0)
        shadow = ShadowCalculation(oFragPosition, Camera.view, normal, lightDirection);

    vec3 lighting = CalculateLighting(oFragPosition, Camera.position.xyz, normal, texColor.rgb, uDiffuseColor.rgb, uAmbientStrength, uShininess, shadow);

    if(World.fogEnabled > 0)
    {
        float visibility = GetFogVisibility(World.fogDensity, World.fogGradient, Camera.position.xyz, oFragPosition);
        lighting = mix(World.fogColor.rgb, lighting, visibility);
    }

    lighting = GammaCorrection(lighting).rgb;

    float alpha = uDiffuseColor.a * texColor.a;
    
    FragColor = vec4(lighting, alpha);
})";

    Shader DiffuseShader::Create()
    {
        return Shader(vertex, fragment);
    }
};