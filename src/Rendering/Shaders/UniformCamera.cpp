#include "UniformCamera.hpp"

namespace GravyEngine
{
    static std::string source = R"(layout(std140, binding = 0) uniform uCamera
{
    mat4 view;
    mat4 projection;
    mat4 viewProjection;
    vec4 position;
} Camera;)";

    std::string UniformCamera::GetSource()
    {
        return source;
    }
};