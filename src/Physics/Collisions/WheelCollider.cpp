#include "WheelCollider.hpp"
#include "../../Core/Time.hpp"
#include "../../Core/Debug.hpp"
#include "../../Core/GameObject.hpp"
#include "../../Core/Transform.hpp"
#include "../../System/Mathf.hpp"
#include "../../System/Drawing/Color.hpp"
#include "../Physics.hpp"

namespace GravyEngine
{
    WheelCollider::WheelCollider() : GameBehaviour()
    {
        radius = 0.5f;
        restLength = 0.5f;
        springTravel = 0.2f;
        springStiffness = 10000;
        damperStiffness = 1500;

        minLength = 0.0f;
        maxLength = 0.0f;
        lastLength = 0.0f;
        springLength = 0.5f;
        springVelocity = 0.0f;
        springForce = 0.0f;
        damperForce = 0.0f;

        center = Vector3(0.0f, 0.0f, 0.0f);
        suspensionForce = Vector3(0.0f, 0.0f, 0.0f);
        velocity = Vector3(0.0f, 0.0f, 0.0f);
        forceDirection = Vector3(0.0f, 0.0f, 0.0f);

        isEnabled = true;

        SetName("WheelCollider");
    }

    void WheelCollider::OnInitialize()
    {
        rigidBody = GetTransform()->GetRoot()->GetGameObject()->GetComponent<Rigidbody>();
    }

    void WheelCollider::OnFixedUpdate()
    {
        if(rigidBody == nullptr)
            return;

        if(!isEnabled)
            return;

        SetMinMaxLength();

        Transform *transform = GetTransform();

        Vector3 offset = GetWheelOffset();
        Vector3 start = transform->GetPosition() + offset;
        Vector3 end = start + (-transform->GetUp() * (maxLength + radius));

        RaycastHit hit;

        if (Physics::RayTest(start, end, hit))
        {
            if(hit.transform->GetRoot() != transform->GetRoot())
            {
                isGrounded = true;
                lastLength = springLength;
                springLength = Mathf::Clamp((hit.distance - radius), minLength, maxLength);
                springVelocity = (lastLength - springLength) / Time::GetFixedDeltaTime();
                springForce = springStiffness * (restLength - springLength);
                damperForce = damperStiffness * springVelocity;
                
                suspensionForce = (springForce + damperForce) * Vector3f::UnitY();

                velocity = rigidBody->GetPointVelocity(hit.point);

                float fX = 0.0f * springForce;
                float fY = velocity.x * springForce;

                Vector3 force = suspensionForce + (fX * transform->GetForward()) + (fY * -transform->GetRight());

                Vector3 localPosition = (hit.point - rigidBody->GetCenterOfMass());

                rigidBody->AddImpulse(force, localPosition);                
            }
            else
            {
                isGrounded = false;
            }
        }
        else
        {
            isGrounded = false;
        }
    }

    void WheelCollider::SetMinMaxLength()
    {
        minLength = restLength - springTravel;
        maxLength = restLength + springTravel;
    }

    void WheelCollider::DrawDebugLines()
    {
        Transform *transform = GetTransform();

        Vector3 offset = GetWheelOffset();
        Vector3 start = transform->GetPosition() + offset;
        Vector3 end = start + (-transform->GetUp() * (springLength + radius));

        Debug::DrawLine(start, end, Color::White());
        
        Quaternion rotation = transform->GetRotation();
        DrawCircle(start, radius, rotation);

        end = start + (forceDirection * 2.0f);
        Debug::DrawLine(start, end, Color::Red());
    }

    void WheelCollider::DrawCircle(const Vector3 &center, float radius, const Quaternion &rotation)
    {
        int segments = 24;

        Quaternion additionalRotation = Quaternionf::FromAxisAngle(Vector3f::UnitY(), 90 * Mathf::Deg2Rad); // add 90 degrees around the y-axis
        auto rot = rotation * additionalRotation; // apply the additional rotation        

        Vector3 xVector = rot * Vector3f::UnitX() * radius;
        Vector3 yVector = rot * Vector3f::UnitY() * radius;

        for (int i = 0; i < segments; i++)
        {
            float angle = i * Mathf::PI * 2 / segments;
            float nextAngle = (i + 1) * Mathf::PI * 2 / segments;

            Vector3 start = center + Mathf::Cos(angle) * xVector + Mathf::Sin(angle) * yVector;
            Vector3 end = center + Mathf::Cos(nextAngle) * xVector + Mathf::Sin(nextAngle) * yVector;

            Debug::DrawLine(start, end, Color::White());
        }
    }

    Vector3 WheelCollider::GetWheelOffset() const
    {
        Transform *transform = GetTransform();
        Vector3 center = GetCenter();
        Vector3 offset =    center.x * transform->GetRight() +
                            center.y * transform->GetUp() +
                            center.z * transform->GetForward();
        return offset;
    }

    void WheelCollider::SetRadius(float radius)
    {
        this->radius = radius;
    }

    float WheelCollider::GetRadius() const
    {
        return radius;
    }

    void WheelCollider::SetRestLength(float restLength)
    {
        this->restLength = restLength;
    }

    float WheelCollider::GetRestLength() const
    {
        return restLength;
    }

    void WheelCollider::SetSpringTravel(float springTravel)
    {
        this->springTravel = springTravel;
    }

    float WheelCollider::GetSpringTravel() const
    {
        return springTravel;             
    }

    void WheelCollider::SetSpringStiffness(float springStiffness)
    {
        this->springStiffness = springStiffness;
    }

    float WheelCollider::GetSpringStiffness() const
    {
        return springStiffness;
    }

    void WheelCollider::SetDamperStiffness(float damperStiffness)
    {
        this->damperStiffness = damperStiffness;
    }

    float WheelCollider::GetDamperStiffness() const
    {
        return damperStiffness;
    }

    void WheelCollider::SetCenter(const Vector3 &center)
    {
        this->center = center;
    }
    
    Vector3 WheelCollider::GetCenter() const
    {
        return center;
    }

    bool WheelCollider::GetIsGrounded() const
    {
        return isGrounded;
    }

    void WheelCollider::SetIsEnabled(bool isEnabled)
    {
        this->isEnabled = isEnabled;    
    }

    bool WheelCollider::GetIsEnabled() const
    {
        return isEnabled;
    }
};