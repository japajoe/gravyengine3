#include "PhysicsManager.hpp"
#include "../Core/Debug.hpp"
#include "../Core/GameObject.hpp"
#include "../Core/Transform.hpp"
#include "../Core/GameBehaviourManager.hpp"
#include "../Core/Time.hpp"
#include "../System/Numerics/Vector3.hpp"
#include "../System/Mathf.hpp"
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>
#include <cstdint>

namespace GravyEngine
{
    int PhysicsManager::fixedTimeStep = 50;
    float PhysicsManager::elapsedTime = 0.0f;
    std::unique_ptr<btBroadphaseInterface> PhysicsManager::broadphase = nullptr;
    std::unique_ptr<btDefaultCollisionConfiguration> PhysicsManager::collisionConfiguration = nullptr;
    std::unique_ptr<btCollisionDispatcher> PhysicsManager::dispatcher = nullptr;
    std::unique_ptr<btSequentialImpulseConstraintSolver> PhysicsManager::solver = nullptr;
    std::unique_ptr<btDiscreteDynamicsWorld> PhysicsManager::dynamicsWorld = nullptr;
    std::vector<btRigidBody*> PhysicsManager::bodies;
    std::vector<Rigidbody*> PhysicsManager::destroyQueue;

    void PhysicsManager::Initialize()
    {
        broadphase = std::make_unique<btDbvtBroadphase>();

        collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
        dispatcher = std::make_unique<btCollisionDispatcher>(collisionConfiguration.get());
        solver = std::make_unique<btSequentialImpulseConstraintSolver>();
        dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(dispatcher.get(), broadphase.get(), solver.get(), collisionConfiguration.get());
        dynamicsWorld->setGravity(btVector3(0,-9.81f,0));
    }

    void PhysicsManager::Deinitialize()
    {
        for(size_t i = 0; i < bodies.size(); i++)
        {
            if(!bodies[i])
                continue;
            dynamicsWorld->removeRigidBody(bodies[i]);
            Rigidbody *g = reinterpret_cast<Rigidbody*>(bodies[i]->getUserPointer());
            g->Destroy();
        }

        bodies.clear();
    }

    void PhysicsManager::OnFixedUpdate()
    {
        elapsedTime += Time::GetDeltaTime();

        float interval = 1.0f / fixedTimeStep;

        if (elapsedTime >= interval)
        {
            Time::OnFixedUpdate();
            GameBehaviourManager::OnFixedUpdate();
            elapsedTime = 0.0f;

            dynamicsWorld->stepSimulation(Time::GetDeltaTime(), (int)(Mathf::Max((float)fixedTimeStep / Time::GetFPS(), 10)));
            //dynamicsWorld->stepSimulation(Time::GetDeltaTime(), 1, interval);

            for (size_t i = 0; i < bodies.size(); i++)
            {
                btRigidBody *body = bodies[i];

                if(!body)
                    continue;

                Rigidbody *g = reinterpret_cast<Rigidbody*>(body->getUserPointer());

                if(!g)
                    continue;
                
                if(!g->GetGameObject()->GetIsActive())
                    continue;

                btMotionState *motionState = body->getMotionState();

                if(!motionState)
                    continue;

                btTransform worldTransform;
                motionState->getWorldTransform(worldTransform);
                btVector3 &origin = worldTransform.getOrigin();
                btQuaternion rotation = worldTransform.getRotation();

                Transform *transform = g->GetTransform();
                transform->SetPosition(Vector3(origin.x(), origin.y(), origin.z()));
                transform->SetRotation(Quaternion(rotation.w(), rotation.x(), rotation.y(), rotation.z()));
            }
        }
    }

    void PhysicsManager::Add(Rigidbody *body)
    {
        if(!body)
        {
            Debug::WriteLog("Can't add rigidbody to PhysicsPipeline because it is null");
            return;
        }
        
        if(!body->Initialize())
        {
            Debug::WriteLog("Can't add rigidbody to PhysicsPipeline because it failed to initialize");
            return;
        }

        Transform *transform = body->GetTransform();

        if(!transform)
        {
            Debug::WriteLog("Can't add rigidbody to PhysicsPipeline because it has no Transform");
            return;
        }

        btRigidBody *rb = body->GetBody();

        if(!rb)
        {
            Debug::WriteLog("Can't add rigidbody to PhysicsPipeline because it has no valid btRigidBody");
            return;
        }

        btCollisionShape *collisionShape = body->GetCollisionShape();

        if(!collisionShape)
        {
            Debug::WriteLog("Can't add rigidbody to PhysicsPipeline because it has no valid btCollisionShape");
            return;
        }

        for(size_t i = 0; i < bodies.size(); i++)
        {
            if(bodies[i] == rb)
            {
                Debug::WriteLog("Can't add rigidbody to PhysicsPipeline because it already exists");
                return;
            }
        }
        
        bodies.push_back(rb);
        dynamicsWorld->addRigidBody(rb);
    }

    void PhysicsManager::Remove(Rigidbody *body)
    {
        if(!body)
            return;

        btRigidBody *rb = body->GetBody();

        if(!rb)
            return;

        size_t index = 0;
        bool found = false;

        for(size_t i = 0; i < bodies.size(); i++)
        {
            if(bodies[i] == rb)
            {
                index = i;
                found = true;
                break;
            }
        }

        if(found)
        {
            destroyQueue.push_back(body);
        }

        if(destroyQueue.size() > 0)
        {
            for(size_t i = 0; i < destroyQueue.size(); i++)
            {
                Rigidbody *body = destroyQueue[i];

                if(body == nullptr)
                    continue;

                if(body->GetBody() == nullptr)
                    continue;

                size_t index = 0;
                bool found = false;

                for(size_t j = 0; j < bodies.size(); j++)
                {
                    if(bodies[j] == body->GetBody())
                    {
                        index = j;
                        found = true;
                        break;
                    }
                }

                if(found)
                {
                    dynamicsWorld->removeRigidBody(body->GetBody());
                    bodies.erase(bodies.begin() + index);
                    body->Destroy();
                }
            }

            destroyQueue.clear();
        }
    }

    bool PhysicsManager::RayTest(const Vector3 &startPoint, const Vector3 &endPoint, RaycastHit &hit)
    {
        if(!dynamicsWorld)
            return false;

        btVector3 from(startPoint.x, startPoint.y, startPoint.z);
        btVector3 to(endPoint.x, endPoint.y, endPoint.z);


        btCollisionWorld::ClosestRayResultCallback callback(from, to);
        dynamicsWorld->rayTest(from, to, callback);

        if(callback.hasHit())
        {
            hit.point = Vector3(callback.m_hitPointWorld.x(), callback.m_hitPointWorld.y(), callback.m_hitPointWorld.z());            
            Rigidbody *rb = reinterpret_cast<Rigidbody*>(callback.m_collisionObject->getUserPointer());
            hit.transform = rb->GetTransform();
            hit.distance = Vector3f::Distance(startPoint, hit.point);
            hit.normal = Vector3(callback.m_hitNormalWorld.x(), callback.m_hitNormalWorld.y(), callback.m_hitNormalWorld.z());
            return true;
        }

        return false;
    }
};