#include "Vector3.hpp"
#include "../Mathf.hpp"

namespace GravyEngine
{
    glm::vec3 Vector3f::UnitX()
    {
        return glm::vec3(1.0f, 0.0f, 0.0f);
    }

    glm::vec3 Vector3f::UnitY()
    {
        return glm::vec3(0.0f, 1.0f, 0.0f);
    }

    glm::vec3 Vector3f::UnitZ()
    {
        return glm::vec3(0.0f, 0.0f, 1.0f);
    }

    glm::vec3 Vector3f::Zero()
    {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    glm::vec3 Vector3f::One()
    {
        return glm::vec3(1.0f, 1.0f, 1.0f);
    }

    glm::vec3 Vector3f::Clamp(const glm::vec3 &v, const glm::vec3 &min, const glm::vec3 &max)
    {
        return glm::clamp(v, min, max);
    }

    glm::vec3 Vector3f::Cross(const glm::vec3 &v1, const glm::vec3 &v2)
    {
        return glm::cross(v1, v2);
    }

    float Vector3f::Dot(const glm::vec3 &v1, const glm::vec3 &v2)
    {
        return glm::dot(v1, v2);
    }

    float Vector3f::Distance(const glm::vec3 &v1, const glm::vec3 &v2)
    {
        return glm::distance(v1, v2);
    }

    float Vector3f::DistanceSquared(const glm::vec3 &v1, const glm::vec3 &v2)
    {
        return glm::distance2(v1, v2);
    }

    float Vector3f::Length(const glm::vec3 &v)
    {
        return glm::length(v);
    }

    float Vector3f::LengthSquared(const glm::vec3 &v)
    {
        return glm::length2(v);
    }

    glm::vec3 Vector3f::Lerp(const glm::vec3 &v1, const glm::vec3 &v2, float t)
    {
        return glm::lerp(v1, v2, t);
    }

    glm::vec3 Vector3f::Slerp(const Vector3 &a, const Vector3 &b, float t)
    {
        // Dot product - the cosine of the angle between 2 vectors.
        float dot = Vector3f::Dot(a, b);
        // Clamp it to be in the range of Acos()
        // This may be unnecessary, but floating point
        // precision can be a fickle mistress.
        dot = glm::clamp(dot, -1.0f, 1.0f);
        // Acos(dot) returns the angle between start and end,
        // And multiplying that by percent returns the angle between
        // start and the final result.
        float theta = glm::acos(dot) * t;
        Vector3 RelativeVec = b - a * dot;
        RelativeVec = Vector3f::Normalize(RelativeVec);
        
        // Orthonormal basis
        // The final result.
        Vector3 a1 = a * glm::cos(theta);
        Vector3 b1 = RelativeVec * glm::sin(theta);

        return a1 + b1;
    }

    glm::vec3 Vector3f::Max(const glm::vec3 &lhs, const glm::vec3 &rhs)
    {
        return glm::vec3(Mathf::Max(lhs.x, rhs.x), Mathf::Max(lhs.y, rhs.y), Mathf::Max(lhs.z, rhs.z));
    }

    glm::vec3 Vector3f::Min(const glm::vec3 &lhs, const glm::vec3 &rhs)
    {
        return glm::vec3(Mathf::Min(lhs.x, rhs.x), Mathf::Min(lhs.y, rhs.y), Mathf::Min(lhs.z, rhs.z));
    }

    glm::vec3 Vector3f::Normalize(const glm::vec3 &v)
    {
        return glm::normalize(v);
    }

    glm::vec3 Vector3f::Transform(const glm::vec3 &v, const glm::quat &r)
    {
        const float x = r.x + r.x;
        const float y = r.y + r.y;
        const float z = r.z + r.z;
        const float wx = r.w * x;
        const float wy = r.w * y;
        const float wz = r.w * z;
        const float xx = r.x * x;
        const float xy = r.x * y;
        const float xz = r.x * z;
        const float yy = r.y * y;
        const float yz = r.y * z;
        const float zz = r.z * z;
        return glm::vec3(
            v.x * (1.0f - yy - zz) + v.y * (xy - wz) + v.z * (xz + wy),
            v.x * (xy + wz) + v.y * (1.0f - xx - zz) + v.z * (yz - wx),
            v.x * (xz - wy) + v.y * (yz + wx) + v.z * (1.0f - xx - yy));
    }

    glm::vec3 Vector3f::TransformPosition(const glm::vec3 &v, const glm::mat4 &m)
    {
        //glm::mat4 mat = glm::transpose(m);
        glm::mat4 mat = m;
        
        glm::vec3 result;
            result.x = (v.x * mat[0][0]) +
                       (v.y * mat[0][1]) +
                       (v.z * mat[0][2]) +
                              mat[0][3];

            result.y = (v.x * mat[1][0]) +
                       (v.y * mat[1][1]) +
                       (v.z * mat[1][2]) +
                              mat[1][3];

            result.z = (v.x * mat[2][0]) +
                       (v.y * mat[2][1]) +
                       (v.z * mat[2][2]) +
                              mat[2][3];
        return result;
    }

    glm::vec3 Vector3f::TransformVector(const glm::vec3 &v, const glm::mat4 &m)
    {
        glm::vec4 homogenousVec(v, 1.0f);
        glm::vec4 transformedVec = m * homogenousVec;
        return glm::vec3(transformedVec);
    }

    bool Vector3f::IsNan(const glm::vec3 &v)
    {
        if(std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z))
            return true;
        return false;
    }

    std::string Vector3f::ToString(const glm::vec3 &v)
    {
        std::string s = std::to_string(v.x) + "," + 
                        std::to_string(v.y) + "," + 
                        std::to_string(v.z);
        return s;
    }
};