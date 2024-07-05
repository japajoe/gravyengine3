#include "Frustum.hpp"
#include "../Core/Camera.hpp"
#include "../Core/Screen.hpp"
#include "../System/Mathf.hpp"
#include <cstring>

namespace GravyEngine
{
    Frustum::Frustum()
    {
        std::memset(planes, 0, 6 * sizeof(Vector3));
    }

    void Frustum::Initialize(const Camera *camera)
    {
        // float far = camera->GetFarClippingPlane();
        // float near = camera->GetNearClippingPlane();
        // float fov = camera->GetFieldOfView();
        // const auto transform = camera->GetTransform();
        // Vector3 position = transform->GetPosition();
        // Vector3 forward = transform->GetForward();
        // Vector3 right = transform->GetRight();
        // Vector3 up = transform->GetUp();

        // auto screenSize = Screen::GetSize();
        // float aspect = screenSize.x / screenSize.y;
        
        // const float halfVSide = far * Mathf::Tan(fov * 0.5f);
        // const float halfHSide = halfVSide * aspect;
        // const Vector3 frontMultFar = far * forward;

        // nearFace = Plane(position + near * forward, forward);
        // farFace = Plane(position + frontMultFar, -forward);
        // rightFace = Plane(position, Vector3f::Cross(frontMultFar - right * halfHSide, up));
        // leftFace = Plane(position, Vector3f::Cross(up,frontMultFar + right * halfHSide));
        // topFace = Plane(position, Vector3f::Cross(right, frontMultFar - up * halfVSide));
        // bottomFace = Plane(position, Vector3f::Cross(frontMultFar + up * halfVSide, right));
    }

    void Frustum::Initialize(const Matrix4 &viewProjection)
    {
        planes[0] = Vector4(
            viewProjection[0][3] + viewProjection[0][0],
            viewProjection[1][3] + viewProjection[1][0],
            viewProjection[2][3] + viewProjection[2][0],
            viewProjection[3][3] + viewProjection[3][0]
        );
        // Right plane
        planes[1] = Vector4(
            viewProjection[0][3] - viewProjection[0][0],
            viewProjection[1][3] - viewProjection[1][0],
            viewProjection[2][3] - viewProjection[2][0],
            viewProjection[3][3] - viewProjection[3][0]
        );
        // Bottom plane
        planes[2] = Vector4(
            viewProjection[0][3] + viewProjection[0][1],
            viewProjection[1][3] + viewProjection[1][1],
            viewProjection[2][3] + viewProjection[2][1],
            viewProjection[3][3] + viewProjection[3][1]
        );
        // Top plane
        planes[3] = Vector4(
            viewProjection[0][3] - viewProjection[0][1],
            viewProjection[1][3] - viewProjection[1][1],
            viewProjection[2][3] - viewProjection[2][1],
            viewProjection[3][3] - viewProjection[3][1]
        );
        // Near plane
        planes[4] = Vector4(
            viewProjection[0][2],
            viewProjection[1][2],
            viewProjection[2][2],
            viewProjection[3][2]
        );
        // Far plane
        planes[5] = Vector4(
            viewProjection[0][3] - viewProjection[0][2],
            viewProjection[1][3] - viewProjection[1][2],
            viewProjection[2][3] - viewProjection[2][2],
            viewProjection[3][3] - viewProjection[3][2]
        );

        // Normalize the planes
        for (size_t i = 0; i < 6; ++i) 
        {
            planes[i] = Vector4f::Normalize(planes[i]);
        }
    }

    bool Frustum::Contains(const BoundingBox &bounds)
    {
        const Vector3 vmin = bounds.GetMin();
        const Vector3 vmax = bounds.GetMax();

        for (size_t i = 0; i < 6; ++i) 
        {
            Vector4 plane = planes[i];
            Vector3 normal = Vector3(plane);
            float d = plane.w;

            Vector3 v;
            v.x = (normal.x > 0.0f) ? vmin.x : vmax.x;
            v.y = (normal.y > 0.0f) ? vmin.y : vmax.y;
            v.z = (normal.z > 0.0f) ? vmin.z : vmax.z;

            if (Vector3f::Dot(normal, v) + d < 0.0f) 
            {
                // Bounding box is outside frustum plane
                return false;
            }
        }

        return true;
    }
};