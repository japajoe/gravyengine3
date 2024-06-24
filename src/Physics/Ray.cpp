#include "Ray.hpp"
#include "../System/Numerics/Matrix4.hpp"
#include "../Core/Camera.hpp"

namespace GravyEngine
{
    Ray::Ray()
    {
        origin = Vector3f::Zero();
        direction = -Vector3f::UnitZ();
        length = 10000;
    }

    Ray::Ray(const Vector3 &origin, const Vector3 &direction, float length)
    {
        this->origin = origin;
        this->direction = direction;
        this->length = length;
    }

    Ray Ray::FromMousePosition(const Vector2 &mousePosition, const Vector4 &viewportRect)
    {
        Vector2 viewportPosition(viewportRect.x, viewportRect.y);
        Vector2 relativeMousePosition = mousePosition - viewportPosition;

        float screenWidth = viewportRect.z;
        float screenHeight = viewportRect.w;

        float mouseX = relativeMousePosition.x;
        float mouseY = screenHeight - relativeMousePosition.y;

        Camera *mainCamera = Camera::GetMain();
        Matrix4 viewMatrix = mainCamera->GetViewMatrix();
        Matrix4 projectionMatrix = mainCamera->GetProjectionMatrix();

        Vector4 rayStartNDC(
            (mouseX / screenWidth - 0.5f) * 2.0f,  // [0,1024] -> [-1,1]
            (mouseY / screenHeight - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
            -1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
            1.0f
        );

        Vector4 rayEndNDC(
            (mouseX /  screenWidth - 0.5f) * 2.0f,
            (mouseY /  screenHeight - 0.5f) * 2.0f,
            0.0,
            1.0f
        );

        Matrix4 m = Matrix4f::Invert(projectionMatrix * viewMatrix);
        Vector4 rayStartWorld = m * rayStartNDC; 
        rayStartWorld /= rayStartWorld.w;
        Vector4 rayEndWorld = m * rayEndNDC;
        rayEndWorld /= rayEndWorld.w;

        Vector3 rayDirWorld(rayEndWorld - rayStartWorld);
        rayDirWorld = Vector3f::Normalize(rayDirWorld);

        Vector3 origin = Vector3(rayStartWorld);
        Vector3 direction = Vector3f::Normalize(rayDirWorld);

        return Ray(origin, direction, 10000);
    }
};
