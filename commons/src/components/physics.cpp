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
 * @file physics.cpp

#include "components/physics.hpp"
#include "debugging/assert.hpp"
#include "core/project.hpp"

using namespace COMMONS_NS;

physics::physics(bool static, const shapes e_forma) : e_forma(e_forma) {
    m_massa = static ? 0 : 1;

    switch (e_forma) {
        case shapes::box_shape:
            m_forma = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
            break;
        case shapes::forma_esfera:
            m_forma = new btSphereShape(0.5f);
            break;
        case shapes::forma_capsula:
            m_forma = new btCapsuleShape(0.5f, 1.8f);
            break;
        case shapes::forma_cilindro:
            m_forma = new btCylinderShape(btVector3(0.5f, 0.5f, 0.5f));
            break;
        case shapes::forma_cone:
            m_forma = new btConeShape(0.5f, 1.f);
            break;
        case shapes::forma_plano:
            m_forma = new btStaticPlaneShape(btVector3(0.f, 1.f, 0.f), 1.f);
            break;
        case shapes::mesh_shape:
            m_forma = nullptr;
            break;
    }


    btVector3 inertia(0, 0, 0);
    if(m_massa > 0) m_forma->calculateLocalInertia(m_massa, inertia);
    m_estado_de_movimento = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(m_massa, m_estado_de_movimento, m_forma, inertia);
    m_rigid_body = new btRigidBody(rigidBodyCI);
}

// Destrutor
physics::~physics() {
    if(m_rigid_body && current_project)current_project->physics_system()->remove(m_rigid_body);
    if(m_rigid_body)delete m_rigid_body;
    if(m_estado_de_movimento)delete m_estado_de_movimento;
    if(m_forma)delete m_forma;
}

// Criar forma para múltiplas meshes
void physics::setModelo(model* p_modelo) {
    if(!p_modelo) throw std::runtime_error("Física: model inválido.");
    m_modelo = p_modelo;
    if(e_forma != shapes::mesh_shape) return;

    btTriangleIndexVertexArray* indexVertexArray = new btTriangleIndexVertexArray();

    for (const auto& mesh : m_modelo->meshes) {
        const auto& vertices = mesh.getVertices();
        const auto& indices = mesh.getIndices();

        btIndexedMesh mesh;
        mesh.m_numTriangles = indices.size() / 3;
        mesh.m_triangleIndexBase = (const unsigned char*)indices.data();
        mesh.m_triangleIndexStride = 3 * sizeof(unsigned int);
        mesh.m_numVertices = vertices.size();
        mesh.m_vertexBase = (const unsigned char*)vertices.data();
        mesh.m_vertexStride = sizeof(vertex);
        mesh.m_indexType = PHY_INTEGER;
        mesh.m_vertexType = PHY_FLOAT;

        indexVertexArray->addIndexedMesh(mesh, PHY_INTEGER);
    }

    m_forma = new btBvhTriangleMeshShape(indexVertexArray, true);
}

// Aplicar força
void physics::setForca(const fvector_type3& vector) {
    m_rigid_body->activate();
    m_rigid_body->applyCentralForce(vector.to_btvec());
}

// Aplicar speed
void physics::setVelocidade(const fvector_type3& speed) {
    m_rigid_body->activate();
    m_rigid_body->setLinearVelocity(speed.to_btvec());
}

// Obter speed
fvector_type3 physics::getVelocidade() const {
    return { m_rigid_body->getLinearVelocity().getX(),
            m_rigid_body->getLinearVelocity().getY(),
            m_rigid_body->getLinearVelocity().getZ() };
}
void physics::setFatorLinear(const fvector_type3& fator) {
        m_rigid_body->activate();
        m_rigid_body->setLinearFactor(fator.to_btvec());
}

void physics::setFatorAngular(const fvector_type3& fator){
        m_rigid_body->activate();
        m_rigid_body->setAngularFactor(fator.to_btvec());
}

void physics::setRestituicao(const float fator) {
        m_rigid_body->activate();
        m_rigid_body->setRestitution(fator);
}
void physics::setFriccao(const float fator) {
        m_rigid_body->activate();
        m_rigid_body->setFriction(fator);
}
void physics::setRaioCCD(const float fator) {
    m_rigid_body->activate();
    m_rigid_body->setCcdSweptSphereRadius(fator);
}
 */
