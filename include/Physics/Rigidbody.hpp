#ifndef GRAVYENGINE_RIGIDBODY_HPP
#define GRAVYENGINE_RIGIDBODY_HPP

#ifdef GRAVY_ENABLE_BULLET

#include <memory>
#include "../System/Numerics/Vector3.hpp"
#include "../System/Numerics/Quaternion.hpp"
#include "../Core/Component.hpp"

#include <vector>

class btCollisionShape;
class btTriangleIndexVertexArray;
class btRigidBody;
class btCollisionShape;
struct btDefaultMotionState;

namespace GravyEngine
{
    class Transform;
    class Collider;

    enum class ForceMode
    {
        Force,          //Add a continuous force to the rigidbody, using its mass.
        Impulse        //Add an instant force impulse to the rigidbody, using its mass.
    };

    class ShapeInfo
    {
    public:
        btCollisionShape *collisionShape;
        btTriangleIndexVertexArray *indexVertexArrays;
        Vector3 position;
        Quaternion rotation;

        ShapeInfo()
        {
            this->collisionShape = nullptr;
            this->indexVertexArrays = nullptr;
            this->position = Vector3(0, 0, 0);
            this->rotation = Quaternion(1, 0, 0, 0);
        }
        ShapeInfo(btCollisionShape *collisionShape, btTriangleIndexVertexArray *indexVertexArrays, Vector3 position, Quaternion rotation)
        {
            this->collisionShape = collisionShape;
            this->indexVertexArrays = indexVertexArrays;
            this->position = position;
            this->rotation = rotation;
        }
    };

    class Rigidbody : public Component
    {
    friend class PhysicsManager;
    public:
        Rigidbody();
        btRigidBody *GetBody() const;
        btCollisionShape *GetCollisionShape() const;
        btDefaultMotionState *GetMotionState() const;
        void SetMass(float mass);
        float GetMass() const;
        void SetBounciness(float bounciness);
        float GetBounciness() const;
        void SetFriction(float friction);
        float GetFriction() const;
        Vector3 GetPointVelocity(const Vector3 &relativePosition);
        void SetVelocity(const Vector3 &velocity);
        Vector3 GetVelocity() const;
        void SetAngularVelocity(const Vector3 &velocity);
        Vector3 GetAngularVelocity() const;
        void SetDrag(float drag);
        float GetDrag() const;
        void SetAngularDrag(float angularDrag);
        float GetAngularDrag() const;
        void SetCenterOfMass(const Vector3 &centerOfMass);
        Vector3 GetCenterOfMass() const;
        void AddForce(const Vector3 &force, ForceMode forceMode = ForceMode::Impulse);
        void AddRelativeForce(const Vector3 &force, ForceMode forceMode = ForceMode::Impulse);
        void AddTorque(const Vector3 &torque, ForceMode forceMode = ForceMode::Impulse);
        void AddRelativeTorque(const Vector3 &torque, ForceMode forceMode = ForceMode::Impulse);
        void AddForceAtPosition(const Vector3 &force, const Vector3 &relativePosition);
        void AddImpulse(const Vector3 &impulse, const Vector3 &relativePosition);
        void MovePosition(const Vector3 &position);
        void MoveRotation(const Quaternion &rotation);
        void FreezeRotation(const Vector3 &axes);
        void Activate();
    protected:
        void OnDestroy() override;
        void OnActivate() override;
        void OnDeactivate() override;
        void Destroy();
    private:
        btRigidBody* body;
        btCollisionShape* collisionShape;
        btDefaultMotionState* motionState;
        std::vector<ShapeInfo> childShapes;
        float mass;
        float drag;
        float angularDrag;
        Vector3 centerOfMass;
        bool Initialize();
        bool InitializeCollisionShape();
        bool CreateShape(Collider *c, ShapeInfo &shape);
    };
};

#endif //GRAVY_ENABLE_BULLET

#endif