#ifndef GRAVYENGINE_VECTOR3_HPP
#define GRAVYENGINE_VECTOR3_HPP

#include "../../External/glm/glm.h"
#include <string>

namespace GravyEngine
{
    typedef glm::vec3 Vector3;

    class Vector3f
    {
    public:
        static glm::vec3 UnitX();
        static glm::vec3 UnitY();
        static glm::vec3 UnitZ();
        static glm::vec3 Zero();
        static glm::vec3 One();
        static glm::vec3 Cross(const glm::vec3 &v1, const glm::vec3 &v2);
        static float Dot(const glm::vec3 &v1, const glm::vec3 &v2);
        static float Distance(const glm::vec3 &v1, const glm::vec3 &v2);
        static float DistanceSquared(const glm::vec3 &v1, const glm::vec3 &v2);
        static float Length(const glm::vec3 &v);
        static float LengthSquared(const glm::vec3 &v);
        static glm::vec3 Lerp(const glm::vec3 &v1, const glm::vec3 &v2, float t);
        static glm::vec3 Slerp(const Vector3 &a, const Vector3 &b, float t);
        static glm::vec3 Max(const glm::vec3 &lhs, const glm::vec3 &rhs);
        static glm::vec3 Min(const glm::vec3 &lhs, const glm::vec3 &rhs);
        static glm::vec3 Normalize(const glm::vec3 &v);
        static glm::vec3 Transform(const glm::vec3 &v, const glm::quat &r);
        static glm::vec3 TransformPosition(const glm::vec3 &v, const glm::mat4 &m);
        static glm::vec3 TransformVector(const glm::vec3 &v, const glm::mat4 &m);
        static bool IsNan(const glm::vec3 &v);
        static std::string ToString(const glm::vec3 &v);
    };
};

#endif