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
 * @file sistema_de_fisica.cpp
 */

#include "nucleo/engine.hpp"
#include "sistemas/sistema_de_fisica.hpp"
#include "depuracao/debug.hpp"
#include "nucleo/engine.hpp"
#include "os/janela.hpp"
#include "componentes/transformacao.hpp"
#include "componentes/fisica.hpp"

using namespace becommons;

sistema_fisica::sistema_fisica() {
    configColisao = new btDefaultCollisionConfiguration();
    expedidor = new btCollisionDispatcher(configColisao);
    faseAmpla = new btDbvtBroadphase();
    solucionador = new btSequentialImpulseConstraintSolver();
    mundoDinamico = new btDiscreteDynamicsWorld(expedidor, faseAmpla, solucionador, configColisao);
    mundoDinamico->setGravity(m_gravity);
}
sistema_fisica::~sistema_fisica() {
    if (mundoDinamico) {
        // Remove todos os corpos antes de deletar o mundo
        for (int i = mundoDinamico->getNumCollisionObjects() - 1; i >= 0; --i) {
            btCollisionObject* obj = mundoDinamico->getCollisionObjectArray()[i];
            btRigidBody* body = btRigidBody::upcast(obj);
            if (body && body->getMotionState()) {
                delete body->getMotionState();
            }
            mundoDinamico->removeCollisionObject(obj);
            delete obj;
        }
        delete mundoDinamico;
        mundoDinamico = nullptr;
    }
    if(solucionador)delete solucionador;
    if(faseAmpla)delete faseAmpla;
    if(expedidor)delete expedidor;
    if(configColisao)delete configColisao;
}

void sistema_fisica::atualizarColisoes() {
    mundoDinamico->updateAabbs();
    mundoDinamico->performDiscreteCollisionDetection();
}
void sistema_fisica::atualizar() {
    auto reg = motor::obter().m_levelmanager->obterFaseAtual()->obterRegistro();
    // Atualiza o Bullet com o transform da engine
    reg->cada<fisica, transformacao>([&](uint32_t entidade) {
        auto f = reg->obter<fisica>(entidade);
        auto t = reg->obter<transformacao>(entidade);
    
        // Se o corpo não for dinâmico ou foi movido manualmente, sincroniza
        if (f->m_corpo_rigido) { 
            btTransform trans;
            trans.setOrigin(btVector3(t->posicao.x, t->posicao.y, t->posicao.z));
                
            // Converte rotação conforme sua convenção (aqui supondo quaternion)
            btQuaternion quaternion;
            quaternion.setEulerZYX(t->rotacao.y, t->rotacao.x, t->rotacao.z);
            
            trans.setRotation(quaternion);
    
            f->m_corpo_rigido->setWorldTransform(trans);
            if (f->m_corpo_rigido->getMotionState())
                f->m_corpo_rigido->getMotionState()->setWorldTransform(trans);
        }
    });

    // atualiza
    mundoDinamico->stepSimulation(motor::obter().m_tempo->obterDeltaTime(), m_depth);

    // Atualiza a transformacao
    reg->cada<fisica, transformacao>([&](const uint32_t entidade) {
        auto f = reg->obter<fisica>(entidade);
        auto t = reg->obter<transformacao>(entidade);
        btTransform bt;
        f->m_estado_de_movimento->getWorldTransform(bt);

        t->posicao = fvet3(bt.getOrigin());
        btScalar yaw, pitch, roll;
        bt.getRotation().getEulerZYX(yaw, pitch, roll);
        t->rotacao = fvet3(pitch, yaw, roll);
    });
}

void sistema_fisica::inicializar() {
    sistema::inicializar();
    depuracao::emitir(info, "fisica", "iniciando sistema.");
    auto reg = motor::obter().m_levelmanager->obterFaseAtual()->obterRegistro();
    
    if (mundoDinamico) {
        for (int i = mundoDinamico->getNumCollisionObjects() - 1; i >= 0; --i) {
            btCollisionObject* obj = mundoDinamico->getCollisionObjectArray()[i];
            mundoDinamico->removeCollisionObject(obj);
        }
        delete mundoDinamico;
    }

    mundoDinamico = new btDiscreteDynamicsWorld(expedidor, faseAmpla, solucionador, configColisao);
    mundoDinamico->setGravity(btVector3(0, -9.8, 0));
    
    reg->cada<fisica, transformacao>([reg, this](const uint32_t entidade) {
            /// adiciona corpos rigidos
            auto comp_fisica = reg->obter<fisica>(entidade);
            auto comp_transf = reg->obter<transformacao>(entidade);

            /* set position */
            comp_fisica->definirPosicao(comp_transf->posicao);
            btCollisionShape* shape = comp_fisica->m_corpo_rigido->getCollisionShape();

            /* set scale */
            btVector3 scale = shape->getLocalScaling();
            scale.setX(comp_transf->escala.x); // Set X-axis scale
            scale.setY(comp_transf->escala.y); // Set Y-axis scale
            scale.setZ(comp_transf->escala.z); // Set Z-axis scale
            
            /* set rot */
            btTransform trans;
            comp_fisica->m_estado_de_movimento->getWorldTransform(trans);
            
            btQuaternion quaternion;
            quaternion.setEulerZYX(comp_transf->rotacao.y, comp_transf->rotacao.x, comp_transf->rotacao.z);
            trans.setRotation(quaternion);
    
            comp_fisica->m_corpo_rigido->setWorldTransform(trans);
            if (comp_fisica->m_corpo_rigido->getMotionState())
                comp_fisica->m_corpo_rigido->getMotionState()->setWorldTransform(trans);
            shape->setLocalScaling(scale);
            /* add body */
            mundoDinamico->addRigidBody(comp_fisica->m_corpo_rigido, comp_fisica->m_camada, comp_fisica->m_camada);
        }
    );
}

bool sistema_fisica::remover(btRigidBody*& corpo)
{
    if (!mundoDinamico) {
        return false; // Retorna falso se o mundo não existir
    }

    if (!corpo) {
        return false; // Retorna falso se o corpo for inválido
    }

    if (mundoDinamico->getCollisionObjectArray().findLinearSearch(corpo) == mundoDinamico->getNumCollisionObjects()) {
        return false; // Retorna falso se o corpo não estiver no mundo
    }

    mundoDinamico->removeRigidBody(corpo);
    corpo = nullptr; // Evita uso de ponteiro inválido

    return true;
}

btDiscreteDynamicsWorld* sistema_fisica::mundo()
{
    return mundoDinamico;
}

resultadoRaio sistema_fisica::emitirRaio(const raio& raio)
{
    btVector3 origem(raio.origem.x, raio.origem.y, raio.origem.z);
    btVector3 destino = origem + btVector3(raio.direcao.x, raio.direcao.y, raio.direcao.z) * 300.0f;

    btCollisionWorld::ClosestRayResultCallback callback(origem, destino);

    mundoDinamico->rayTest(origem, destino, callback);

    resultadoRaio resultado = { false, fvet3(0.0f), fvet3(0.0f), nullptr };

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
