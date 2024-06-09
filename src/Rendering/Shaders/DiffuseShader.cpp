#include "DiffuseShader.hpp"

namespace GravyEngine
{
    static std::string vertex = R"(#version 420 core
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

    static std::string fragment = R"(#version 420 core

#include <ShaderCore>
//#include <UniformCamera>
//#include <UniformLight>
//#include <UniformShadow>

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

    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    for(int i = 0; i < MAX_NUM_LIGHTS; i++)
    {
        if(Lights.lights[i].isActive > 0)
        {
            LightInfo light = Lights.lights[i];

            vec3 lightPosition = light.position.xyz;
            vec3 lightColor = light.color.rgb;
            float lightStrength = light.strength;
            float attenuation = 1.0;
            
            vec3 lightDir = vec3(0.0);

            if(Lights.lights[i].type == 0) //Directional
            {
                lightDir = normalize(light.direction.xyz);
            }
            else //Point
            {
                lightDir = normalize(lightPosition - oFragPosition);
                float lightLinear = light.linear;
                float lightConstant = light.constant;
                float lightQuadratic = light.quadratic;
                float distance  = length(lightPosition - oFragPosition);
                attenuation = 1.0 / (lightConstant + lightLinear * distance + lightQuadratic * (distance * distance));
            }

            // ambient
            ambient += light.ambient.rgb * uAmbientStrength * texColor.rgb * attenuation;

            // diffuse
            float diff = max(dot(lightDir, normal), 1.0);
            //float diff = 1.0;
            diffuse += light.diffuse.rgb * diff * lightColor * lightStrength * attenuation;

            // specular
            vec3 viewDir = normalize(Camera.position.xyz - oFragPosition);
            vec3 halfwayDir = normalize(lightDir + viewDir);  
            float spec = pow(max(dot(normal, halfwayDir), 0.0), uShininess);
            specular += light.specular.rgb * spec * lightColor * lightStrength * attenuation;
        }
    }

    vec3 lightDirection = normalize(Lights.lights[0].direction.xyz);
    //vec3 lightDirection = normalize(Lights.lights[0].position.xyz - oFragPosition);

    float shadow  = 0.0;
    if(uReceiveShadows > 0)
        shadow = ShadowCalculation(oFragPosition, Camera.view, normal, lightDirection);

    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * texColor.rgb * uDiffuseColor.rgb;

    if(World.fogEnabled > 0)
    {
        float visibility = GetFogVisibility(World.fogDensity, World.fogGradient, Camera.position.xyz, oFragPosition);
        lighting = mix(World.fogColor.rgb, lighting, visibility);
    }

    lighting = GammaCorrection(lighting).rgb;

    float alpha = uDiffuseColor.a * texColor.a;
    
    FragColor = vec4(lighting, alpha);
})";

    static const char *name = "Diffuse";

    Shader *DiffuseShader::Create()
    {
        if(Shader::Find(name) != nullptr)
            return nullptr;

        Shader shader(vertex, fragment);
        return Shader::Add(name, shader);
    }

    void DiffuseShader::Destroy()
    {
        Shader::Remove(name);
    }
};