#include "Transformation.hpp"

namespace GravyEngine
{
    Transformation::Transformation()
    {
        transformation = Matrix4f::Identity();
    }
    
    Transformation::Transformation(const Matrix4 &transformation)
    {
        this->transformation = transformation;
    }
    
    void Transformation::SetMatrix(const Matrix4 &transformation)
    {
        this->transformation = transformation;
    }

    Matrix4 Transformation::GetMatrix() const
    {
        return transformation;
    }

    Vector3 Transformation::GetTranslation()
    {
        return Matrix4f::ExtractTranslation(transformation);
    }

    Quaternion Transformation::GetRotation()
    {
        return Matrix4f::ExtractRotation(transformation);
    }

    Vector3 Transformation::GetScale()
    {
        return Matrix4f::ExtractScale(transformation);
    }

    Vector3 Transformation::WorldToLocal(const Vector3 &v)
    {
        auto invScale = GetScale();
        if (invScale.x != 0.0f)
            invScale.x = 1.0f / invScale.x;
        if (invScale.y != 0.0f)
            invScale.y = 1.0f / invScale.y;
        if (invScale.z != 0.0f)
            invScale.z = 1.0f / invScale.z;
        const Quaternion invRotation = Quaternionf::Conjugate(GetRotation());
        Vector3 result = v - GetTranslation();
        result = Vector3f::Transform(result, invRotation);
        result *= invScale;
        return result;
    }

    Vector3 Transformation::WorldToLocalVector(const Vector3 &v)
    {
        auto invScale = GetScale();
        if (invScale.x != 0.0f)
            invScale.x = 1.0f / invScale.x;
        if (invScale.y != 0.0f)
            invScale.y = 1.0f / invScale.y;
        if (invScale.z != 0.0f)
            invScale.z = 1.0f / invScale.z;
        const Quaternion invRotation = Quaternionf::Conjugate(GetRotation());
        Vector3 result = Vector3f::Transform(v, invRotation);
        result *= invScale;
        return result;
    }

    Vector3 Transformation::LocalToWorld(const Vector3 &v)
    {
        Vector3 tmp = v * GetScale();
        tmp = Vector3f::Transform(tmp, GetRotation());
        return tmp + GetTranslation();
    }

    Vector3 Transformation::LocalToWorldVector(const Vector3 &v)
    {
        Vector3 tmp = v * GetScale();
        return Vector3f::Transform(tmp, GetRotation());
    }
};