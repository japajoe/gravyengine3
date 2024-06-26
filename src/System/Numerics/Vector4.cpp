#include "Vector4.hpp"
#include "../Mathf.hpp"

namespace GravyEngine
{
    float Vector4f::Dot(const glm::vec4 &v1, const glm::vec4 &v2)
    {
        return glm::dot(v1, v2);
    }

    float Vector4f::Distance(const glm::vec4 &v1, const glm::vec4 &v2)
    {
        return glm::distance(v1, v2);
    }

    float Vector4f::DistanceSquared(const glm::vec4 &v1, const glm::vec4 &v2)
    {
        return glm::distance2(v1, v2);
    }

    float Vector4f::Length(const glm::vec4 &v)
    {
        return glm::length(v);
    }

    float Vector4f::LengthSquared(const glm::vec4 &v)
    {
        return glm::length2(v);
    }

    glm::vec4 Vector4f::Lerp(const glm::vec4 &v1, const glm::vec4 &v2, float t)
    {
        return glm::lerp(v1, v2, t);
    }

    glm::vec4 Vector4f::Max(const glm::vec4 &lhs, const glm::vec4 &rhs)
    {
        return glm::vec4(Mathf::Max(lhs.x, rhs.x), Mathf::Max(lhs.y, rhs.y), Mathf::Max(lhs.z, rhs.z), Mathf::Max(lhs.w, rhs.w));
    }

    glm::vec4 Vector4f::Min(const glm::vec4 &lhs, const glm::vec4 &rhs)
    {
        return glm::vec4(Mathf::Min(lhs.x, rhs.x), Mathf::Min(lhs.y, rhs.y), Mathf::Min(lhs.z, rhs.z), Mathf::Min(lhs.w, rhs.w));
    }

    glm::vec4 Vector4f::Normalize(const glm::vec4 &v)
    {
        return glm::normalize(v);
    }

    bool Vector4f::IsNan(const glm::vec4 &v)
    {
        if(std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z) || std::isnan(v.w))
            return true;
        return false;
    }

    std::string Vector4f::ToString(const glm::vec4 &v)
    {
        std::string s = std::to_string(v.x) + "," + 
                        std::to_string(v.y) + "," + 
                        std::to_string(v.z) + "," + 
                        std::to_string(v.w);
        return s;
    }
};