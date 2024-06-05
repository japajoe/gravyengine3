#include "DiffuseShader.hpp"

namespace GravyEngine
{
    static std::string vertex = R"(#version 330 core

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
        oNormal = uModelInverted * aNormal;
        oFragPosition = vec3(uModel * vec4(aPosition, 1.0));
        oUV = aUV;
        gl_Position = uMVP * vec4(aPosition, 1.0);
    })";

    static std::string fragment = R"(#version 330 core

    uniform sampler2D uDiffuseTexture;
    uniform vec2 uUVOffset;
    uniform vec2 uUVScale;

    in vec3 oNormal;
    in vec3 oFragPosition;
    in vec2 oUV;

    out vec4 FragColor;

    void main() {
        vec4 texColor = texture(uDiffuseTexture, (oUV + uUVOffset) * uUVScale);
        FragColor = texColor;
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