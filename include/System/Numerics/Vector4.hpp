#ifndef GRAVYENGINE_VECTOR4_HPP
#define GRAVYENGINE_VECTOR4_HPP

#include "../../External/glm/glm.h"
#include <string>

namespace GravyEngine
{
    typedef glm::vec4 Vector4;

    class Vector4f
    {
    public:
        static float Dot(const glm::vec4 &v1, const glm::vec4 &v2);
        static float Distance(const glm::vec4 &v1, const glm::vec4 &v2);
        static float DistanceSquared(const glm::vec4 &v1, const glm::vec4 &v2);
        static float Length(const glm::vec4 &v);
        static float LengthSquared(const glm::vec4 &v);
        static glm::vec4 Lerp(const glm::vec4 &v1, const glm::vec4 &v2, float t);
        static glm::vec4 Normalize(const glm::vec4 &v);
        static bool IsNan(const glm::vec4 &v);
        static std::string ToString(const glm::vec4 &v);
    };
};

#endif