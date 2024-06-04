#ifndef GRAVYENGINE_VERTEX_HPP
#define GRAVYENGINE_VERTEX_HPP

#include "../System/Numerics/Vector2.hpp"
#include "../System/Numerics/Vector3.hpp"
#include "../System/Numerics/Vector4.hpp"

namespace GravyEngine
{
    struct Vertex
    {
        Vector3 position;
        Vector3 normal;
        Vector2 uv;
        Vertex();
        Vertex(const Vector3 &position, const Vector3 &normal, const Vector2 &uv);
    };
};

#endif