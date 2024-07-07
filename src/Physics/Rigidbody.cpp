#include "Rigidbody.hpp"
#include "PhysicsManager.hpp"
#include "Collisions/BoxCollider.hpp"
#include "Collisions/CapsuleCollider.hpp"
#include "Collisions/CylinderCollider.hpp"
#include "Collisions/MeshCollider.hpp"
#include "Collisions/SphereCollider.hpp"
#include "Collisions/TerrainCollider.hpp"
#include "../Core/Transform.hpp"
#include "../Core/GameObject.hpp"
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

namespace GravyEngine
{
    Rigidbody::Rigidbody() : Component()
    {
        mass = 0;
        drag = 0.1f;
        angularDrag = 0.1f;
        centerOfMass = Vector3(0, 0, 0);
        body = nullptr;
        collisionShape = nullptr;
        motionState = nullptr;
        SetName("Rigidbody");
    }

    void Rigidbody::OnDestroy()
    {
        PhysicsManager::Remove(this);
    }

    void Rigidbody::OnActivate()
    {
        if(body && collisionShape && motionState)
        {
            body->activate(true);
        }
    }

    void Rigidbody::OnDeactivate()
    {
        if(body && collisionShape && motionState)
        {
            body->setActivationState(ISLAND_SLEEPING);
        }
    }    

    bool Rigidbody::Initialize()
    {
        if(body)
            return false;

        if(!InitializeCollisionShape())
            return false;

        btVector3 localInertia(0, 0, 0);
        if (mass != 0.0f) 
            collisionShape->calculateLocalInertia(mass, localInertia);

        Quaternion orientation = GetTransform()->GetRotation();
        Vector3 position = GetTransform()->GetPosition();
        
        motionState = new btDefaultMotionState(btTransform(
            btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w), 
            btVector3(position.x, position.y, position.z)
        ));

        btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, collisionShape, localInertia);

        body = new btRigidBody(rigidBodyCI);
        body->setUserPointer(this);
        body->setDamping(drag, angularDrag);
        return true;
    }

    bool Rigidbody::InitializeCollisionShape()
    {
        auto colliders = GetTransform()->GetGameObject()->GetComponents<Collider>();

        if(colliders.size() == 0)
            return false;

        if(colliders.size() == 1)
        {
            Collider *c = colliders[0];
            ShapeInfo shape;

            if(!CreateShape(c, shape))
                return false;
            
            collisionShape = shape.collisionShape;
            childShapes.push_back(ShapeInfo(shape.collisionShape, shape.indexVertexArrays, shape.position, shape.rotation));
        }
        else
        {
            //Compound shape
            bool success = true;

            for(size_t i = 0; i < colliders.size(); i++)
            {
                Collider *c = colliders[i];
                ShapeInfo shape;

                if(!CreateShape(c, shape))
                {
                    success = false;
                    break;
                }

                childShapes.push_back(ShapeInfo(shape.collisionShape, shape.indexVertexArrays, shape.position, shape.rotation));
            }

            if(!success)
            {
                for(size_t i = 0; i < childShapes.size(); i++)
                {
                    if(childShapes[i].collisionShape)
                        delete childShapes[i].collisionShape;
                    if(childShapes[i].indexVertexArrays)
                        delete childShapes[i].indexVertexArrays;
                }

                childShapes.clear();
                collisionShape = nullptr;
                return false;
            }

            collisionShape = new btCompoundShape();

            btCompoundShape *compoundShape = static_cast<btCompoundShape*>(collisionShape);

            for(size_t i = 0; i < childShapes.size(); i++)
            {
                btTransform transform;
                transform.setIdentity();
                btVector3 pos(childShapes[i].position.x, childShapes[i].position.y, childShapes[i].position.z);
                btQuaternion rot(childShapes[i].rotation.x, childShapes[i].rotation.y, childShapes[i].rotation.z, childShapes[i].rotation.w);
                transform.setOrigin(pos);
                transform.setRotation(rot);
                compoundShape->addChildShape(transform, childShapes[i].collisionShape);
            }
        }

        return true;
    }

    void Rigidbody::Destroy()
    {
        if(motionState)
            delete motionState;

        if(childShapes.size() > 0)
        {
            for(size_t i = 0; i < childShapes.size(); i++)
            {
                if(childShapes[i].collisionShape)
                {
                    delete childShapes[i].collisionShape;
                }

                if(childShapes[i].indexVertexArrays)
                {
                    delete childShapes[i].indexVertexArrays;
                }
            }
        }
        else
        {
            if(collisionShape)
                delete collisionShape;
        }

        if(body)
            delete body;

        childShapes.clear();
        motionState = nullptr;
        collisionShape = nullptr;
        body = nullptr;
    }

    bool Rigidbody::CreateShape(Collider *c, ShapeInfo &shape)
    {
        shape.collisionShape = nullptr;
        shape.indexVertexArrays = nullptr;

        switch(c->GetType())
        {
            case ColliderType::Box:
            {
                BoxCollider *collider = static_cast<BoxCollider*>(c);
                Vector3 halfExtents = collider->GetSize() * 0.5f;
                shape.collisionShape = new btBoxShape(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
                break;
            }
            case ColliderType::Capsule:
            {
                CapsuleCollider *collider = static_cast<CapsuleCollider*>(c);
                shape.collisionShape = new btCapsuleShape(collider->GetRadius(), collider->GetHeight());
                break;
            }
            case ColliderType::Cylinder:
            {
                CylinderCollider *collider = static_cast<CylinderCollider*>(c);
                Vector3 halfExtents = collider->GetSize() * 0.5f;
                shape.collisionShape = new btCylinderShape(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
                break;
            }
            case ColliderType::Mesh:
            {
                MeshCollider *collider = static_cast<MeshCollider*>(c);
                Mesh *mesh = collider->GetMesh();

                if(!mesh)
                    return false;

                auto &vertices = mesh->GetVertices();
                auto &indices = mesh->GetIndices();

                size_t totalVerts = vertices.size();
                size_t totalTriangles = indices.size() / 3;
                size_t triangleIndexStride = 3 * sizeof(unsigned int);
                size_t vertexStride = sizeof(Vertex);

                btIndexedMesh indexedMesh;
                indexedMesh.m_numTriangles = totalTriangles;
                indexedMesh.m_numVertices = totalVerts;
                indexedMesh.m_triangleIndexStride = triangleIndexStride;
                indexedMesh.m_triangleIndexBase = reinterpret_cast<const unsigned char*>(indices.data());
                indexedMesh.m_vertexStride = vertexStride;
                indexedMesh.m_vertexBase = reinterpret_cast<const unsigned char*>(vertices.data());

                shape.indexVertexArrays = new btTriangleIndexVertexArray();
                shape.indexVertexArrays->addIndexedMesh(indexedMesh);
                shape.collisionShape = new btBvhTriangleMeshShape(shape.indexVertexArrays, false);
                
                break;
            }
            case ColliderType::Sphere:
            {
                SphereCollider *collider = static_cast<SphereCollider*>(c);
                shape.collisionShape = new btSphereShape(collider->GetRadius());
                break;
            }
            default:
            {
                return false;
            }
            // case ColliderType::Terrain:
            // {
            //     TerrainCollider *collider = static_cast<TerrainCollider*>(c);

            //     Mesh *mesh = collider->GetMesh();

            //     if(!mesh)
            //         return false;

            //     if(collider->GetWidth() == 0 || collider->GetDepth() == 0)
            //         return false;

            //     //shape.collisionShape = new btHeightfieldTerrainShape()
            // }
        }
        
        shape.position = c->GetCenter();
        shape.rotation = Quaternion(1, 0, 0, 0);

        return true;
    }

    btRigidBody *Rigidbody::GetBody() const
    {
        return body;
    }

    btCollisionShape *Rigidbody::GetCollisionShape() const
    {
        return collisionShape;
    }

    btDefaultMotionState *Rigidbody::GetMotionState() const
    {
        return motionState;
    }

    void Rigidbody::SetMass(float mass)
    {
        this->mass = mass;
        if(body)
        {
            btVector3 inertia = body->getLocalInertia();
            body->setMassProps(mass, inertia);
        }
    }

    float Rigidbody::GetMass() const
    {
        return mass;
    }

    void Rigidbody::SetBounciness(float bounciness)
    {
        if(!body)
            return;
        body->setRestitution(bounciness);
    }

    float Rigidbody::GetBounciness() const
    {
        if(!body)
            return 0.0f;
        return body->getRestitution();
    }

    void Rigidbody::SetFriction(float friction)
    {
        if(!body)
            return;
        body->setFriction(friction);
    }

    float Rigidbody::GetFriction() const
    {
        if(!body)
            return 0.0f;
        return body->getFriction();
    }

    Vector3 Rigidbody::GetPointVelocity(const Vector3 &relativePosition)
    {
        if(!body)
            return Vector3(0, 0, 0);
        auto com = body->getCenterOfMassPosition();
        btVector3 relPos(relativePosition.x - com.x(), relativePosition.y - com.y(), relativePosition.z - com.z());
        btVector3 result = body->getPushVelocityInLocalPoint(relPos);
        return Vector3(result.x(), result.y(), result.z());
    }

    void Rigidbody::SetVelocity(const Vector3 &velocity)
    {
        if(!body)
            return;
        Activate();
        body->setLinearFactor(btVector3(velocity.x, velocity.y, velocity.z));
    }

    Vector3 Rigidbody::GetVelocity() const
    {
        if(!body)
            return Vector3(0, 0, 0);
        btVector3 velocity = body->getLinearVelocity();
        return Vector3(velocity.x(), velocity.y(), velocity.z());
    }

    void Rigidbody::SetAngularVelocity(const Vector3 &velocity)
    {
        if(!body)
            return;
        Activate();
        body->setAngularVelocity(btVector3(velocity.x, velocity.y, velocity.z));
    }

    Vector3 Rigidbody::GetAngularVelocity() const
    {
        if(!body)
            return Vector3(0, 0, 0);
        btVector3 velocity = body->getAngularVelocity();
        return Vector3(velocity.x(), velocity.y(), velocity.z());
    }

    void Rigidbody::SetDrag(float drag)
    {
        this->drag = drag;
        if(body)
        {
            body->setDamping(drag, body->getAngularDamping());
        }
    }

    float Rigidbody::GetDrag() const
    {
        if(!body)
            return drag;
        return body->getLinearDamping();
    }

    void Rigidbody::SetAngularDrag(float angularDrag)
    {
        this->angularDrag = angularDrag;
        if(body)
        {
            body->setDamping(body->getLinearDamping(), angularDrag);
        }
    }

    float Rigidbody::GetAngularDrag() const
    {
        if(!body)
            return angularDrag;
        return body->getAngularDamping();
    }

    void Rigidbody::SetCenterOfMass(const Vector3 &centerOfMass)
    {
        this->centerOfMass = centerOfMass;        
        if(!body)
            return;        

        btTransform centerOfMassTransform;
        centerOfMassTransform.setIdentity();
        centerOfMassTransform.setOrigin(btVector3(centerOfMass.x, centerOfMass.y, centerOfMass.z));        
        body->setCenterOfMassTransform(centerOfMassTransform);

        btVector3 inertia;
        body->getCollisionShape()->calculateLocalInertia(mass, inertia);
        body->setMassProps(mass, inertia);
    }

    Vector3 Rigidbody::GetCenterOfMass() const
    {
        if(!body)
            return centerOfMass;
        
        auto pos = body->getCenterOfMassPosition();
        return Vector3(pos.x(), pos.y(), pos.z());
    }

    void Rigidbody::AddForce(const Vector3 &force, ForceMode forceMode)
    {
        if(!body)
            return;
        Activate();
        if(forceMode == ForceMode::Impulse)
            body->applyCentralImpulse(btVector3(force.x, force.y, force.z));
        else if(forceMode == ForceMode::Force)
            body->applyCentralForce(btVector3(force.x, force.y, force.z));
    }

    void Rigidbody::AddRelativeForce(const Vector3 &force, ForceMode forceMode)
    {
        if(!body)
            return;
        Activate();
        auto forc = GetTransform()->TransformDirection(force);
        if(forceMode == ForceMode::Impulse)
            body->applyCentralImpulse(btVector3(forc.x, forc.y, forc.z));
        else if(forceMode == ForceMode::Force)
            body->applyCentralForce(btVector3(forc.x, forc.y, forc.z));
    }

    void Rigidbody::AddTorque(const Vector3 &torque, ForceMode forceMode)
    {
        if(!body)
            return;
        Activate();
        if(forceMode == ForceMode::Impulse)
            body->applyTorqueImpulse(btVector3(torque.x, torque.y, torque.z));
        else if(forceMode == ForceMode::Force)
            body->applyTorque(btVector3(torque.x, torque.y, torque.z));
    }

    void Rigidbody::AddRelativeTorque(const Vector3 &torque, ForceMode forceMode)
    {
        if(!body)
            return;
        Activate();

        auto torq = GetTransform()->TransformDirection(torque);

        if(forceMode == ForceMode::Impulse)
            body->applyTorqueImpulse(btVector3(torq.x, torq.y, torq.z));
        else if(forceMode == ForceMode::Force)
            body->applyTorque(btVector3(torq.x, torq.y, torq.z));
    }

    void Rigidbody::AddForceAtPosition(const Vector3 &force, const Vector3 &relativePosition)
    {
        if(!body)
            return;
        Activate();
        body->applyForce(btVector3(force.x, force.y, force.z), btVector3(relativePosition.x, relativePosition.y, relativePosition.z));
    }

    void Rigidbody::AddImpulse(const Vector3 &impulse, const Vector3 &relativePosition)
    {
        if(!body)
            return;
        Activate();
        body->applyImpulse(btVector3(impulse.x, impulse.y, impulse.z), btVector3(relativePosition.x, relativePosition.y, relativePosition.z));
    }

    void Rigidbody::MovePosition(const Vector3 &position)
    {
        if(!body)
            return;
        Activate();
        auto &worldTransform = body->getWorldTransform();
        worldTransform.setOrigin(btVector3(position.x, position.y, position.z));
    }

    void Rigidbody::MoveRotation(const Quaternion &rotation)
    {
        if(!body)
            return;
        Activate();
        auto &worldTransform = body->getWorldTransform();
        worldTransform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
    }

    void Rigidbody::Activate()
    {
        if(!body)
            return;
        auto state = body->getActivationState();

        switch (state)
        {

            case ISLAND_SLEEPING:
                body->activate(true);
            case ACTIVE_TAG:
            case WANTS_DEACTIVATION:
            case DISABLE_DEACTIVATION:
            case DISABLE_SIMULATION:
            default:
                break;
        }
    }
};