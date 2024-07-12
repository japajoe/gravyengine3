#ifndef GRAVYENGINE_PHYSICSMANAGER_HPP
#define GRAVYENGINE_PHYSICSMANAGER_HPP

#ifdef GRAVY_ENABLE_BULLET

#include "Rigidbody.hpp"
#include "RaycastHit.hpp"
#include "../System/Numerics/Matrix4.hpp"
#include "../System/Numerics/Vector3.hpp"
#include <memory>

class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btDynamicsWorld;
class btRigidBody;

namespace GravyEngine
{
    class PhysicsManager
    {
    public:
        static void Initialize();
        static void Deinitialize();
        static void OnFixedUpdate();
        static void Add(Rigidbody *body);
        static void Remove(Rigidbody *body);
        static bool RayTest(const Vector3 &startPoint, const Vector3 &endPoint, RaycastHit &hit);
    private:
        static int fixedTimeStep;
        static float elapsedTime;
        static std::unique_ptr<btBroadphaseInterface> broadphase;	
	    static std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
	    static std::unique_ptr<btCollisionDispatcher> dispatcher;	
	    static std::unique_ptr<btSequentialImpulseConstraintSolver> solver;	
	    static std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
        static std::vector<btRigidBody*> bodies;
        static std::vector<Rigidbody*> destroyQueue;
    };
};

#endif //GRAVY_ENABLE_BULLET

#endif