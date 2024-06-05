#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Component.hpp"
#include "../System/Numerics/Matrix4.hpp"
#include "../System/Drawing/Color.hpp"

namespace GravyEngine
{
    enum class ProjectionMode
    {
        Orthographic,
        Perspective
    };

    class Camera : public Component
    {
    public:
        Camera();
        Matrix4 GetProjectionMatrix() const;
        Matrix4 GetViewMatrix() const;
        void SetProjectionMode(ProjectionMode mode);
        ProjectionMode GetProjectionMode() const;
        void SetFieldOfView(float fieldOfView);
        float GetFieldOfView() const;
        void SetNearClippingPlane(float nearClippingPlane);
        float GetNearClippingPlane() const;
        void SetFarClippingPlane(float farClippingPlane);
        float GetFarClippingPlane() const;
        void SetClearColor(const Color &clearColor);
        Color GetClearColor() const;
        static Camera *GetMain();
    protected:
        void OnInitialize() override;
    private:
        static Camera *pMainCamera;
        ProjectionMode mode;
        Matrix4 projection;
        Color clearColor;
        float fieldOfView;
        float nearClippingPlane;
        float farClippingPlane;
        void Initialize();
        static void OnScreenResize(int width, int height);
    };
};

#endif