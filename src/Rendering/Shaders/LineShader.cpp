#include "LineShader.hpp"

namespace GravyEngine
{
    static std::string vertex = R"(#version 420 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;

uniform mat4 uMVP;

out vec4 oColor;

void main()
{
    oColor = aColor;
    gl_Position = uMVP * vec4(aPosition, 1.0);
})";

    static std::string fragment = R"(#version 420 core
#include <ShaderCore>

in vec4 oColor;
out vec4 FragColor;

void main()
{
    FragColor = oColor;
    FragColor = GammaCorrection(FragColor.xyz);
    FragColor.a = oColor.a;
})";

    static const char *name = "Line";

    Shader *LineShader::Create()
    {
        if(Shader::Find(name) != nullptr)
            return nullptr;

        Shader shader(vertex, fragment);
        return Shader::Add(name, shader);
    }

    void LineShader::Destroy()
    {
        Shader::Remove(name);
    }
};