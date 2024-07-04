#include "Frustum.hpp"
#include "../Core/Camera.hpp"
#include "../Core/Screen.hpp"
#include "../System/Mathf.hpp"

namespace GravyEngine
{
    Frustum::Frustum()
    {

    }

    void Frustum::Initialize(const Camera *camera)
    {
        float far = camera->GetFarClippingPlane();
        float near = camera->GetNearClippingPlane();
        float fov = camera->GetFieldOfView();
        const auto transform = camera->GetTransform();
        Vector3 position = transform->GetPosition();
        Vector3 forward = transform->GetForward();
        Vector3 right = transform->GetRight();
        Vector3 up = transform->GetUp();

        auto screenSize = Screen::GetSize();
        float aspect = screenSize.x / screenSize.y;
        
        const float halfVSide = far * Mathf::Tan(fov * 0.5f);
        const float halfHSide = halfVSide * aspect;
        const Vector3 frontMultFar = far * forward;

        nearFace = Plane(position + near * forward, forward);
        farFace = Plane(position + frontMultFar, -forward);
        rightFace = Plane(position, Vector3f::Cross(frontMultFar - right * halfHSide, up));
        leftFace = Plane(position, Vector3f::Cross(up,frontMultFar + right * halfHSide));
        topFace = Plane(position, Vector3f::Cross(right, frontMultFar - up * halfVSide));
        bottomFace = Plane(position, Vector3f::Cross(frontMultFar + up * halfVSide, right));

    }
};