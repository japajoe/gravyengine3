#include "Camera.hpp"
#include "Screen.hpp"
#include "Transform.hpp"

namespace GravyEngine
{
    static bool setCallback = true;
    Camera *Camera::pMainCamera = nullptr;

    Camera::Camera() : Component()
    {
        this->mode = ProjectionMode::Perspective;
        this->fieldOfView = 70.0f;
        this->nearClippingPlane = 0.1f;
        this->farClippingPlane = 1000.0f;
        this->clearColor = Color::White();
    }

    void Camera::OnInitialize()
    {
        if(pMainCamera == nullptr)
            pMainCamera = this;

        if(setCallback)
        {
            Screen::resize += OnScreenResize;
            setCallback = false;
        }

        Initialize();
    }

    void Camera::Initialize()
    {
        auto screenSize = Screen::GetSize();
        float aspectRatio = screenSize.x / screenSize.y;

        if(mode == ProjectionMode::Orthographic)
            projection = Matrix4f::Orthographic(0.0f, screenSize.x, 0.0f, screenSize.y, nearClippingPlane, farClippingPlane);
        else
            projection = Matrix4f::Perspective(fieldOfView, aspectRatio, nearClippingPlane, farClippingPlane);
    }

    Matrix4 Camera::GetProjectionMatrix() const
    {
        return projection;
    }

    Matrix4 Camera::GetViewMatrix() const
    {
        return Matrix4f::Invert(GetTransform()->GetModelMatrix());
    }

    void Camera::SetProjectionMode(ProjectionMode mode)
    {
        this->mode = mode;
        Initialize();
    }

    ProjectionMode Camera::GetProjectionMode() const
    {
        return mode;
    }

    void Camera::SetFieldOfView(float fieldOfView)
    {
        this->fieldOfView = fieldOfView;
        Initialize();
    }

    float Camera::GetFieldOfView() const
    {
        return fieldOfView;
    }

    void Camera::SetNearClippingPlane(float nearClippingPlane)
    {
        this->nearClippingPlane = nearClippingPlane;
        Initialize();
    }

    float Camera::GetNearClippingPlane() const
    {
        return nearClippingPlane;
    }

    void Camera::SetFarClippingPlane(float farClippingPlane)
    {
        this->farClippingPlane = farClippingPlane;
        Initialize();
    }

    float Camera::GetFarClippingPlane() const
    {
        return farClippingPlane;
    }

    void Camera::SetClearColor(const Color &clearColor)
    {
        this->clearColor = clearColor;
    }

    Color Camera::GetClearColor() const
    {
        return clearColor;
    }

    Camera *Camera::GetMain()
    {
        return pMainCamera;
    }

    void Camera::OnScreenResize(int width, int height)
    {
        if(pMainCamera)
            pMainCamera->Initialize();
    }
};
