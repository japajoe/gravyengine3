#include "DepthShader.hpp"

namespace GravyEngine
{
    static std::string vertex = 
R"(#version 330 core
layout (location = 0) in vec3 aPosition;

uniform mat4 uModel;

void main()
{
    gl_Position = uModel * vec4(aPosition, 1.0);
})";

    static std::string geometry = 
R"(#version 420 core
layout(triangles, invocations = 5) in;
layout(triangle_strip, max_vertices = 3) out;

#include <ShaderCore>

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

    static std::string fragment = 
R"(#version 330 core
void main()
{             
})";

    static std::string geometryGLSL330 =
R"(#version 420 core

//#include <UniformShadow>
#include <ShaderCore>

// Geometry shader for processing the input primitives
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

void main()
{
    for (int invocation = 0; invocation < 5; ++invocation)
    {
        for (int i = 0; i < 3; ++i)
        {
            gl_Position = Shadow.lightSpaceMatrices[invocation] * gl_in[i].gl_Position;
            gl_Layer = invocation;
            EmitVertex();
        }
        EndPrimitive();
    }
})";

    Shader DepthShader::Create()
    {
        return Shader(vertex, fragment, geometry);
    }
};