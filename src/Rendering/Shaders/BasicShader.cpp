#include "BasicShader.hpp"

namespace GravyEngine
{
    static std::string vertex = 
R"(#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aUV;

out vec2 oUV;

void main()
{
    oUV = aUV;
    gl_Position = vec4(aPosition.x, aPosition.y, 0.0, 1.0); 
})";

    static std::string fragment = 
R"(#version 330 core

uniform sampler2D uDiffuseTexture;

in vec2 oUV;

out vec4 FragColor;

void main()
{
    vec4 col = texture(uDiffuseTexture, oUV);
    FragColor = col;
})";

    Shader BasicShader::Create()
    {
        return Shader(vertex, fragment);
    }
};