#pragma once
#include "system.hpp"
#include <iostream>
#include "util/ray.hpp"
#include <bullet/btBulletDynamicsCommon.h>

class MyContactCallback : public btCollisionWorld::ContactResultCallback {
public:
    int colisoes = 0;
    btScalar addSingleResult 	( 	btManifoldPoint &  	cp,
		const btCollisionObjectWrapper *  	colObj0Wrap,
		int  	partId0,
		int  	index0,
		const btCollisionObjectWrapper *  	colObj1Wrap,
		int  	partId1,
		int  	index1
	) 	 override
    {
        colisoes ++;
        return 0;
    }
};

namespace COMMONS_NS {
    struct physics_system : system {
         physics_system();
        ~physics_system();

        void update(const std::shared_ptr<ecs>&) override;
        void setup(const std::shared_ptr<ecs>&) override;
        bool remove(btRigidBody*& body);
        btDiscreteDynamicsWorld* world();
        float speed;
        ray_result cast_ray(const ray& ray);
    private:
        btDefaultCollisionConfiguration* configColisao{nullptr};
        btCollisionDispatcher* expedidor{nullptr};
        btBroadphaseInterface* faseAmpla{nullptr};
        btSequentialImpulseConstraintSolver* solucionador{nullptr};
        btDiscreteDynamicsWorld* dynamic_world{nullptr};
    };
}
