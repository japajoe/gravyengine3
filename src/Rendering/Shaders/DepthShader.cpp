#include "DepthShader.hpp"

namespace GravyEngine
{
    static std::string vertex = R"(#version 420 core
layout (location = 0) in vec3 aPosition;

uniform mat4 uModel;

void main()
{
    gl_Position = uModel * vec4(aPosition, 1.0);
})";

    static std::string geometry = R"(#version 420 core
layout(triangles, invocations = 5) in;
layout(triangle_strip, max_vertices = 3) out;

#include <UniformShadow>

void main()
{          
	for (int i = 0; i < 3; ++i)
	{
		gl_Position = Shadow.lightSpaceMatrices[gl_InvocationID] * gl_in[i].gl_Position;
		gl_Layer = gl_InvocationID;
		EmitVertex();
	}
	EndPrimitive();
})";

    static std::string fragment = R"(#version 410 core
void main()
{             
})";

    static const char *name = "Depth";

    Shader *DepthShader::Create()
    {
        if(Shader::Find(name) != nullptr)
            return nullptr;

        Shader shader(vertex, fragment, geometry);
        return Shader::Add(name, shader);
    }

    void DepthShader::Destroy()
    {
        Shader::Remove(name);
    }
};