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

#include "becommons_namespace.hpp"
#include "sistemas/sistema_de_fisica.hpp"
#include "depuracao/debug.hpp"
#include "nucleo/fase.hpp"
#include "nucleo/projeto.hpp"
#include "os/janela.hpp"
#include "componentes/transformacao.hpp"
#include "componentes/fisica.hpp"

using namespace BECOMMONS_NS;

sistema_fisica::sistema_fisica() : velocidade(1.f) {
    configColisao = new btDefaultCollisionConfiguration();
    expedidor = new btCollisionDispatcher(configColisao);
    faseAmpla = new btDbvtBroadphase();
    solucionador = new btSequentialImpulseConstraintSolver();
    mundoDinamico = new btDiscreteDynamicsWorld(expedidor, faseAmpla, solucionador, configColisao);
    mundoDinamico->setGravity(btVector3(0, -9.8, 0));
}
sistema_fisica::~sistema_fisica() {
    if(solucionador)delete solucionador;
    if(faseAmpla)delete faseAmpla;
    if(expedidor)delete expedidor;
    if(configColisao)delete configColisao;
    if(mundoDinamico)delete mundoDinamico;
}

void sistema_fisica::atualizar() {
    auto reg = projeto_atual->obterFaseAtual()->obterRegistro();
    reg->cada<fisica, transformacao>([&](const uint32_t entidade) {
        auto f = reg->obter<fisica>(entidade);
        auto t = reg->obter<transformacao>(entidade);

        btTransform bt;
        btQuaternion btRot;
        // define posicao
        bt.setOrigin(t->posicao.to_btvec());
        // define rotação
        btRot.setEulerZYX(t->rotacao.x, t->rotacao.y, t->rotacao.z); 
        bt.setRotation(btRot); 

        f->m_estado_de_movimento->setWorldTransform(bt); 
        f->m_corpo_rigido->activate();
        f->m_corpo_rigido->setMotionState(f->m_estado_de_movimento);
    });
    
    mundoDinamico->stepSimulation(janela::obterInstancia().m_tempo.obterDeltaTime() * velocidade, 1);
    
    reg->cada<fisica, transformacao>([&](const uint32_t entidade) {
        auto f = reg->obter<fisica>(entidade);
        auto t = reg->obter<transformacao>(entidade);
        btTransform bt;
        f->m_estado_de_movimento->getWorldTransform(bt);
        t->posicao = fvet3(bt.getOrigin());
        t->definirRotacao(fvet4(bt.getRotation()));
    });
}

void sistema_fisica::inicializar() {
    auto reg = projeto_atual->obterFaseAtual()->obterRegistro();
    reg->cada<fisica, transformacao>([reg, this](const uint32_t entidade) {
            /// adiciona corpos rigidos
            auto comp_fisica = reg->obter<fisica>(entidade);
            mundoDinamico->addRigidBody(comp_fisica->m_corpo_rigido);
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
