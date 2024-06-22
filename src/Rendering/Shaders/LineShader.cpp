#include "LineShader.hpp"

namespace GravyEngine
{
    static std::string vertex = R"(#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;

uniform mat4 uMVP;

out vec4 oColor;

void main()
{
    oColor = aColor;
    gl_Position = uMVP * vec4(aPosition, 1.0);
})";

    static std::string fragment = R"(#version 330 core

in vec4 oColor;
out vec4 FragColor;

vec4 GammaCorrection(vec3 color)
{
    return vec4(pow(color, vec3(1.0/2.2)), 1.0);
}

void main()
{
    FragColor = oColor;
    FragColor = GammaCorrection(FragColor.xyz);
    FragColor.a = oColor.a;
})";

    Shader LineShader::Create()
    {
        return Shader(vertex, fragment);
    }
};