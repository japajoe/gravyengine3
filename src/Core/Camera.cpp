#include "Camera.hpp"
#include "Screen.hpp"
#include "Transform.hpp"
#include "../Rendering/Buffers/UniformBufferObject.hpp"
#include "../Rendering/Graphics.hpp"

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

        SetDirty(true);
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

    void Camera::SetDirty(bool isDirty)
    {
        this->isDirty = isDirty;
    }

    bool Camera::IsDirty() const
    {
        return isDirty;
    }

    Camera *Camera::GetMain()
    {
        return pMainCamera;
    }

    static UniformBufferObject *uniformBuffer = nullptr;
    static int updateCounter = 0;

    void Camera::UpdateUniformBuffer()
    {
        if(uniformBuffer == nullptr)
        {
            uniformBuffer = Graphics::FindUniformBuffer("Camera");

            if(uniformBuffer == nullptr)
                return;
        }

        Camera *camera = Camera::GetMain();

        if(camera == nullptr)
            return;
        
        bool isDirty = false;
        
        Transform *transform = camera->GetTransform();
        
        if( transform->GetPosition() != camera->transformData.position || 
            transform->GetRotation() != camera->transformData.rotation || 
            transform->GetScale() != camera->transformData.scale || 
            camera->IsDirty())
        {
            camera->transformData.position = transform->GetPosition();
            camera->transformData.rotation = transform->GetRotation();
            camera->transformData.scale = transform->GetScale();
            camera->SetDirty(false);
            isDirty = true;
        }

        if(!isDirty)
            return;

        UniformCameraInfo cameraData;

        cameraData.view = camera->GetViewMatrix();
        cameraData.projection = camera->GetProjectionMatrix();
        cameraData.viewProjection = cameraData.projection * cameraData.view;
        cameraData.position = Vector4(camera->GetTransform()->GetPosition(), 1.0f);

        uniformBuffer->Bind();
        uniformBuffer->BufferSubData(0, sizeof(UniformCameraInfo), &cameraData);
        uniformBuffer->Unbind(); 
    }

    void Camera::OnScreenResize(int width, int height)
    {
        if(pMainCamera)
            pMainCamera->Initialize();
    }
};
