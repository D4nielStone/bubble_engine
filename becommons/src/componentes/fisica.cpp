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
 * @file fisica.cpp
 */

#include "componentes/fisica.hpp"
#include "depuracao/assert.hpp"
#include "depuracao/debug.hpp"
#include "nucleo/projeto.hpp"

using namespace BECOMMONS_NS;

bool fisica::analizar(const rapidjson::Value& value) {
    posicao_inicial = btVector3(0, 0, 0);
    escala_inicial = btVector3(1, 1, 1);
    if(value.HasMember("massa"))
        m_massa = value["massa"].GetFloat();
    if (value.HasMember("posicao") && value["posicao"].IsArray()) {
        const auto& pos = value["posicao"].GetArray();
        if (pos.Size() == 3 && pos[0].IsNumber() && pos[1].IsNumber() && pos[2].IsNumber()) {
            posicao_inicial = btVector3(pos[0].GetFloat(), pos[1].GetFloat(), pos[2].GetFloat());
        }
    }
    if (value.HasMember("escala") && value["escala"].IsArray()) {
        const auto& pos = value["escala"].GetArray();
        if (pos.Size() == 3 && pos[0].IsNumber() && pos[1].IsNumber() && pos[2].IsNumber()) {
            escala_inicial = btVector3(pos[0].GetFloat(), pos[1].GetFloat(), pos[2].GetFloat());
        }
    }
    
    // Define forma bt
    switch (e_forma) {
        case formas::forma_caixa:
            m_forma = new btBoxShape(escala_inicial);
            break;
        case formas::forma_esfera:
            m_forma = new btSphereShape(0.5f);
            break;
        case formas::forma_capsula:
            m_forma = new btCapsuleShape(0.5f, 1.8f);
            break;
        case formas::forma_cilindro:
            m_forma = new btCylinderShape(btVector3(0.5f, 0.5f, 0.5f));
            break;
        case formas::forma_cone:
            m_forma = new btConeShape(0.5f, 1.f);
            break;
        case formas::forma_plano:
            m_forma = new btStaticPlaneShape(btVector3(0.f, 1.f, 0.f), 1.f);
            break;
        case formas::forma_malha:
            m_forma = nullptr;
            break;
    }

    btVector3 inertia(0, 0, 0);
    if(m_massa > 0) m_forma->calculateLocalInertia(m_massa, inertia);
    m_estado_de_movimento = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), posicao_inicial));
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(m_massa, m_estado_de_movimento, m_forma, inertia);
    m_corpo_rigido = new btRigidBody(rigidBodyCI);

    return true;
}
bool fisica::serializar(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const {
    value.AddMember("massa", m_massa, allocator);
    
    // posição Vetor 3
    rapidjson::Value pos(rapidjson::kArrayType);
    fvet3 posicao = fvet3(0.f);
    posicao = projeto_atual->obterFaseAtual()->obterRegistro()->obter<transformacao>(meu_objeto)->posicao;
    pos.PushBack(posicao.x, allocator);
    pos.PushBack(posicao.y, allocator);
    pos.PushBack(posicao.z, allocator);
    value.AddMember("posicao", pos, allocator);
    // escala Vetor 3
    rapidjson::Value esc(rapidjson::kArrayType);
    fvet3 escala = fvet3(1.f);
    escala = projeto_atual->obterFaseAtual()->obterRegistro()->obter<transformacao>(meu_objeto)->escala;
    esc.PushBack(escala.x, allocator);
    esc.PushBack(escala.y, allocator);
    esc.PushBack(escala.z, allocator);
    value.AddMember("escala", esc, allocator);

    return true;
}

fisica::fisica(bool estatico, const formas e_forma) : e_forma(e_forma) {
    // Define massa inicial
    m_massa = estatico ? 0 : 1;
}

// Destrutor
fisica::~fisica() {
    depuracao::emitir(debug, "fisica", "descarregando");
    if(m_corpo_rigido && projeto_atual)projeto_atual->m_fisica.remover(m_corpo_rigido);
    if(m_corpo_rigido)delete m_corpo_rigido;
    if(m_estado_de_movimento)delete m_estado_de_movimento;
    if(m_forma)delete m_forma;
}

// Criar forma para múltiplas malhas
void fisica::definirModelo(modelo* p_modelo) {
    if(!p_modelo) throw std::runtime_error("Física: modelo inválido.");
    m_modelo = p_modelo;
    if(e_forma != formas::forma_malha) return;

    btTriangleIndexVertexArray* indexVertexArray = new btTriangleIndexVertexArray();

    for (const auto& malha : m_modelo->malhas) {
        const auto& vertices = malha.obterVertices();
        const auto& indices = malha.obterIndices();

        btIndexedMesh mesh;
        mesh.m_numTriangles = indices.size() / 3;
        mesh.m_triangleIndexBase = (const unsigned char*)indices.data();
        mesh.m_triangleIndexStride = 3 * sizeof(unsigned int);
        mesh.m_numVertices = vertices.size();
        mesh.m_vertexBase = (const unsigned char*)vertices.data();
        mesh.m_vertexStride = sizeof(vertice);
        mesh.m_indexType = PHY_INTEGER;
        mesh.m_vertexType = PHY_FLOAT;

        indexVertexArray->addIndexedMesh(mesh, PHY_INTEGER);
    }

    m_forma = new btBvhTriangleMeshShape(indexVertexArray, true);
}

// Aplicar força
void fisica::definirForca(const fvet3& vetor) {
    m_corpo_rigido->activate();
    m_corpo_rigido->applyCentralForce(vetor.to_btvec());
}

// Aplicar velocidade
void fisica::definirVelocidade(const fvet3& velocidade) {
    m_corpo_rigido->activate();
    m_corpo_rigido->setLinearVelocity(velocidade.to_btvec());
}

// Obter velocidade
fvet3 fisica::obterVelocidade() const {
    return { m_corpo_rigido->getLinearVelocity().getX(),
            m_corpo_rigido->getLinearVelocity().getY(),
            m_corpo_rigido->getLinearVelocity().getZ() };
}
void fisica::definirFatorLinear(const fvet3& fator) {
        m_corpo_rigido->activate(); 
        m_corpo_rigido->setLinearFactor(fator.to_btvec()); 
}

void fisica::definirFatorAngular(const fvet3& fator){
        m_corpo_rigido->activate(); 
        m_corpo_rigido->setAngularFactor(fator.to_btvec()); 
}

void fisica::definirRestituicao(const float fator) {
        m_corpo_rigido->activate(); 
        m_corpo_rigido->setRestitution(fator); 
}
void fisica::definirFriccao(const float fator) {
        m_corpo_rigido->activate(); 
        m_corpo_rigido->setFriction(fator);
}
void fisica::definirRaioCCD(const float fator) {
    m_corpo_rigido->activate();
    m_corpo_rigido->setCcdSweptSphereRadius(fator);
}
