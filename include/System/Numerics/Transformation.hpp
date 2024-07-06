#ifndef GRAVYENGINE_TRANSFORMATION_HPP
#define GRAVYENGINE_TRANSFORMATION_HPP

#include "../../External/glm/glm.h"
#include "Vector3.hpp"
#include "Quaternion.hpp"
#include "Matrix4.hpp"

namespace GravyEngine
{
    class Transformation
    {
    public:
        Transformation();
        Transformation(const Matrix4 &transformation);
        void SetMatrix(const Matrix4 &transformation);
        Matrix4 GetMatrix() const;
        Vector3 GetTranslation();
        Quaternion GetRotation();
        Vector3 GetScale();
        Vector3 WorldToLocal(const Vector3 &v);
        Vector3 WorldToLocalVector(const Vector3 &v);
        Vector3 LocalToWorld(const Vector3 &v);
        Vector3 LocalToWorldVector(const Vector3 &v);
    private:
        Matrix4 transformation;
    };
};

#endif