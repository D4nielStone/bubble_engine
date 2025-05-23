
/** @copyright 
MIT License
Copyright (c) 2025 Daniel Oliveira

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
*/
/**
 * @file fisica.cpp
 */

#include "componentes/fisica.hpp"
#include "componentes/transformacao.hpp"
#include "nucleo/fase.hpp"
#include "nucleo/projeto.hpp"
#include "componentes/renderizador.hpp"
#include "depuracao/assert.hpp"

using namespace BECOMMONS_NS;

// Construtor para forma gen�rica
fisica::fisica(btCollisionShape* forma, btScalar massa, btVector3 posicaoInicial, camada camada) 
    : forma(forma), usar_malha(false), massa(massa), posicaoInicial(posicaoInicial), camada_colisao(camada)
{
    ASSERT(forma != nullptr);
    init();
}
void fisica::init()
{
    btVector3 inertia(0, 0, 0);
    if (massa > 0) {
        forma->calculateLocalInertia(massa, inertia);
    }

    estadoDeMovimento = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), posicaoInicial));
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(massa, estadoDeMovimento, forma, inertia);
    corpoRigido = new btRigidBody(rigidBodyCI);
    corpoRigido->setRollingFriction(0.1);
    corpoRigido->setRestitution(0.8f);
    corpoRigido->setCcdMotionThreshold(0.01f); // Pequeno movimento necess�rio para ativar o CCD
    corpoRigido->setCcdSweptSphereRadius(0.05f); // Define um raio de varredura para detectar colis�es
}
// Construtor para cria��o de malha
fisica::fisica(bool malha, btScalar massa, btVector3 posicaoInicial, camada camada)
    : usar_malha(malha), massa(0), posicaoInicial(posicaoInicial), camada_colisao(camada)
{
}

// Destrutor
fisica::~fisica()
{
    if(corpoRigido && projeto_atual->obterFaseAtual())projeto_atual->sfisica()->remover(corpoRigido);
    if(corpoRigido)delete corpoRigido;
    if(estadoDeMovimento)delete estadoDeMovimento;
    if(forma)delete forma;
}

// Obter o corpo r�gido
btRigidBody* fisica::obterCorpoRigido()
{

    if (usar_malha &&!corpoRigido) {
        criarMalha();
        init();
        usar_malha = false; // Garante que as malhas sejam criadas apenas uma vez
    }
    return corpoRigido;
}

// Criar forma para m�ltiplas malhas
void fisica::criarMalha()
{
    // Obt�m as malhas do modelo associado ao objeto
    auto modelo = projeto_atual->obterFaseAtual()->obterRegistro()->obter<renderizador>(meu_objeto)->m_modelo;
    auto& malhas = modelo->malhas;

    // Criar o btTriangleIndexVertexArray para todas as malhas
    btTriangleIndexVertexArray* indexVertexArray = new btTriangleIndexVertexArray();

    for (const auto& malha : malhas) {
        // Supondo que malha tenha 'vertices' e 'indices'
        const auto& vertices = malha.obterVertices();
        const auto& indices = malha.obterIndices();

        btIndexedMesh mesh;
        mesh.m_numTriangles = indices.size() / 3;
        mesh.m_triangleIndexBase = (const unsigned char*)indices.data();
        mesh.m_triangleIndexStride = 3 * sizeof(unsigned int);
        mesh.m_numVertices = vertices.size();
        mesh.m_vertexBase = (const unsigned char*)vertices.data();
        mesh.m_vertexStride = sizeof(vertice); // Supondo que Vertex tem posi��es cont�guas
        mesh.m_indexType = PHY_INTEGER;
        mesh.m_vertexType = PHY_FLOAT;

        indexVertexArray->addIndexedMesh(mesh, PHY_INTEGER);
    }

    // Cria a forma de colis�o como um btBvhTriangleMeshShape
    forma = new btBvhTriangleMeshShape(indexVertexArray, true);
}

// Atualizar transforma��o
void fisica::atualizarTransformacao()
{
    m_transformacao = projeto_atual->obterFaseAtual()->obterRegistro()->obter<transformacao>(meu_objeto).get();
    forma->setLocalScaling(m_transformacao->obterEscala().to_btvec());

    if (massa == 0)  return;
    btTransform bt;
    estadoDeMovimento->getWorldTransform(bt);

    m_transformacao->definirPosicao(fvet3(bt.getOrigin()  ));
    m_transformacao->definirRotacao(fvet4(bt.getRotation()));
}

// Aplicar for�a
void fisica::aplicarForca(const fvet3& vetor)
{
    corpoRigido->applyCentralForce({ vetor.x, vetor.y, vetor.z });
}

// Aplicar velocidade
void fisica::aplicarVelocidade(const fvet3& velocidade)
{
    corpoRigido->activate();
    corpoRigido->setLinearVelocity(btVector3(velocidade.x, velocidade.y, velocidade.z));
}

// definir posi��o
void fisica::definirPosicao(const fvet3& posicao)
{
    m_transformacao->definirPosicao(posicao);
    // Ensure the rigid body exists
    if (estadoDeMovimento)
    {
        // Create a new transform with the updated position
        btTransform bt;
        estadoDeMovimento->getWorldTransform(bt); // Get the current transform
        bt.setOrigin(btVector3(posicao.x, posicao.y, posicao.z)); // Set the new position

            estadoDeMovimento->setWorldTransform(bt); // Update the transform
            corpoRigido->setLinearVelocity(btVector3(0, 0, 0)); // Reset velocity to avoid unwanted movement
            corpoRigido->setAngularVelocity(btVector3(0, 0, 0)); // Reset angular velocity
            corpoRigido->activate(); // Reactivate the body if it was sleeping
            corpoRigido->setMotionState(estadoDeMovimento);
        
    }
}

// definir rota��o
void fisica::definirRotacao(const fvet3& rotacao)
{
    m_transformacao->definirRotacao(rotacao);
    btTransform bt;
    estadoDeMovimento->getWorldTransform(bt); // Recupera transforma��o atual
    btQuaternion btRot;
    btRot.setEulerZYX(rotacao.z, rotacao.y, rotacao.x); // Define rota��o com Euler ZYX
    bt.setRotation(btRot); // Aplica rota��o
    estadoDeMovimento->setWorldTransform(bt); // Aplica transforma��o
}

// Obter velocidade
fvet3 fisica::obterVelocidade() const
{
    return { corpoRigido->getLinearVelocity().getX(),
            corpoRigido->getLinearVelocity().getY(),
            corpoRigido->getLinearVelocity().getZ() };
}// Obter velocidade
fvet3 fisica::obterPosicao() const
{
    btTransform bt;
    corpoRigido->getMotionState()->getWorldTransform(bt);
    return { 
        bt.getOrigin().getX(),
        bt.getOrigin().getY(),
        bt.getOrigin().getZ()
    };
}

void fisica::definirFatorLinear(const fvet3& fator)
{
        corpoRigido->setLinearFactor(btVector3(fator.x, fator.y, fator.z)); // Reset velocity to avoid unwanted movement
        corpoRigido->activate(); 
}

void fisica::definirFatorAngular(const fvet3& fator){
        corpoRigido->setAngularFactor(btVector3(fator.x, fator.y, fator.z)); // Reset velocity to avoid unwanted movement
        corpoRigido->activate(); 
}

void fisica::definirRestituicao(const float fator){
        corpoRigido->setRestitution(fator); // Reset velocity to avoid unwanted movement
        corpoRigido->activate(); 
}
void fisica::definirFriccao(const float fator){
        corpoRigido->setFriction(fator); // Reset velocity to avoid unwanted movement
        corpoRigido->activate(); 
}
void fisica::definirRaioCcd(const float fator)
{
    corpoRigido->setContactProcessingThreshold(0.0001f);
    corpoRigido->setCcdMotionThreshold(0.0001f);
    corpoRigido->setCcdSweptSphereRadius(fator);
    corpoRigido->activate();
}
