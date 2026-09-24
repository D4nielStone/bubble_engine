/** @copyright
 * MIT License
 * Copyright (c) 2025 Daniel Oliveira
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * @file physics_system.cpp

#include "commons_namespace.hpp"
#include "systems/physics_system.hpp"
#include "debugging/debug.hpp"
#include "core/phase.hpp"
#include "core/project.hpp"
#include "os/window.hpp"
#include "components/transform.hpp"
#include "components/physics.hpp"

using namespace COMMONS_NS;

physics_system::physics_system() : speed(1.f) {
    configColisao = new btDefaultCollisionConfiguration();
    expedidor = new btCollisionDispatcher(configColisao);
    faseAmpla = new btDbvtBroadphase();
    solucionador = new btSequentialImpulseConstraintSolver();
    dynamic_world = new btDiscreteDynamicsWorld(expedidor, faseAmpla, solucionador, configColisao);
    dynamic_world->setGravity(btVector3(0, -9.8, 0));
}
physics_system::~physics_system() {
    if(solucionador)delete solucionador;
    if(faseAmpla)delete faseAmpla;
    if(expedidor)delete expedidor;
    if(configColisao)delete configColisao;
    if(dynamic_world)delete dynamic_world;
}

void physics_system::update(const std::shared_ptr<ecs>& reg) {
    if (!reg) return;
    reg->cada<physics, transform>([&](const uint32_t entity) {
        auto f = reg->get<physics>(entity);
        auto t = reg->get<transform>(entity);

        btTransform bt;
        btQuaternion btRot;
        // define position
        bt.setOrigin(t->position.to_btvec());
        // define rotação
        btRot.setEulerZYX(t->rotation.x, t->rotation.y, t->rotation.z);
        bt.setRotation(btRot);

        f->m_estado_de_movimento->setWorldTransform(bt);
        f->m_rigid_body->activate();
        f->m_rigid_body->setMotionState(f->m_estado_de_movimento);
    });

    dynamic_world->stepSimulation(window::get_instance().m_time.get_delta_time() * speed, 1);

    reg->cada<physics, transform>([&](const uint32_t entity) {
        auto f = reg->get<physics>(entity);
        auto t = reg->get<transform>(entity);
        btTransform bt;
        f->m_estado_de_movimento->getWorldTransform(bt);
        t->position = fvector_type3(bt.getOrigin());
        t->set_rotation(fvector_type4(bt.getRotation()));
    });
}

void physics_system::setup(const std::shared_ptr<ecs>& reg) {
    if (!reg) return;
    reg->cada<physics, transform>([reg, this](const uint32_t entity) {
            /// adiciona bodys rigidos
            auto comp_fisica = reg->get<physics>(entity);
            dynamic_world->addRigidBody(comp_fisica->m_rigid_body);
        }
    );
}

bool physics_system::remove(btRigidBody*& body)
{
    if (!dynamic_world) {
        return false; // Retorna falso se o world não existir
    }

    if (!body) {
        return false; // Retorna falso se o body for inválido
    }

    if (dynamic_world->getCollisionObjectArray().findLinearSearch(body) == dynamic_world->getNumCollisionObjects()) {
        return false; // Retorna falso se o body não estiver no world
    }

    dynamic_world->removeRigidBody(body);
    body = nullptr; // Evita uso de ponteiro inválido

    return true;
}

btDiscreteDynamicsWorld* physics_system::world()
{
    return dynamic_world;
}

ray_result physics_system::cast_ray(const ray& ray)
{
    btVector3 origem(ray.origem.x, ray.origem.y, ray.origem.z);
    btVector3 destino = origem + btVector3(ray.direction.x, ray.direction.y, ray.direction.z) * 300.0f;

    btCollisionWorld::ClosestRayResultCallback callback(origem, destino);

    dynamic_world->rayTest(origem, destino, callback);

    ray_result resultado = { false, fvector_type3(0.0f), fvector_type3(0.0f), nullptr };

    if (callback.hasHit()) {
        btVector3 ponto = callback.m_hitPointWorld;
        resultado.pontoDeColisao = glm::vec3(ponto.getX(), ponto.getY(), ponto.getZ());

        btVector3 normal = callback.m_hitNormalWorld;
        resultado.normalAtingida = glm::normalize(glm::vec3(normal.getX(), normal.getY(), normal.getZ()));

        resultado.objetoAtingido = callback.m_collisionObject;
        resultado.atingiu = true;
    }

    return resultado;
}
 */
