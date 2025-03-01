#include "componentes/fisica.hpp"
#include "componentes/transformacao.hpp"
#include "nucleo/fase.hpp"
#include "nucleo/projeto.hpp"
#include "componentes/renderizador.hpp"

// Construtor para forma gen�rica
bubble::fisica::fisica(btCollisionShape* forma, btScalar massa, btVector3 posicaoInicial, camada camada) 
    : forma(forma), malha(false), massa(massa), posicaoInicial(posicaoInicial), camada_colisao(camada)
{
    init();
}
void bubble::fisica::init()
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
bubble::fisica::fisica(bool malha, btScalar massa, btVector3 posicaoInicial, camada camada)
    : malha(malha), massa(0), posicaoInicial(posicaoInicial), camada_colisao(camada)
{
}

// Destrutor
bubble::fisica::~fisica()
{
    projeto_atual->fase_atual->sfisica.remover(corpoRigido);
    delete corpoRigido;
    delete estadoDeMovimento;
    delete forma;
}

// Obter o corpo r�gido
btRigidBody* bubble::fisica::obterCorpoRigido()
{

    if (malha &&!corpoRigido) {
        criarMalha();
        init();
        malha = false; // Garante que as malhas sejam criadas apenas uma vez
    }
    return corpoRigido;
}

// Criar forma para m�ltiplas malhas
void bubble::fisica::criarMalha()
{
    // Obt�m as malhas do modelo associado ao objeto
    auto modelo = projeto_atual->fase_atual->obterRegistro()->obter<bubble::renderizador>(meu_objeto)->modelo;
    auto& malhas = modelo->malhas;

    // Criar o btTriangleIndexVertexArray para todas as malhas
    btTriangleIndexVertexArray* indexVertexArray = new btTriangleIndexVertexArray();

    for (const auto& malha : malhas) {
        // Supondo que malha tenha 'vertices' e 'indices'
        const auto& vertices = malha.vertices;
        const auto& indices = malha.indices;

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
void bubble::fisica::atualizarTransformacao()
{
    if (massa == 0)  return;
    btTransform bt;
    estadoDeMovimento->getWorldTransform(bt);

    _Mtransformacao = projeto_atual->fase_atual->obterRegistro()->obter<bubble::transformacao>(meu_objeto).get();
    _Mtransformacao->posicao = { bt.getOrigin().getX(), bt.getOrigin().getY(), bt.getOrigin().getZ() };
    _Mtransformacao->rotacao = { bt.getRotation().getX(), bt.getRotation().getY(), bt.getRotation().getZ() };
}

// Aplicar for�a
void bubble::fisica::aplicarForca(const glm::vec3& vetor)
{
    corpoRigido->applyCentralForce({ vetor.x, vetor.y, vetor.z });
}

// Aplicar velocidade
void bubble::fisica::aplicarVelocidade(const glm::vec3& velocidade)
{
    corpoRigido->activate();
    corpoRigido->setLinearVelocity(btVector3(velocidade.x, velocidade.y, velocidade.z));
}

// definir posi��o
void bubble::fisica::definirPosicao(const glm::vec3& posicao)
{
    _Mtransformacao->posicao = posicao;
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
void bubble::fisica::definirRotacao(const glm::vec3& rotacao)
{
    btTransform bt;
    estadoDeMovimento->getWorldTransform(bt); // Recupera transforma��o atual
    btQuaternion btRot;
    btRot.setEulerZYX(rotacao.z, rotacao.y, rotacao.x); // Define rota��o com Euler ZYX
    bt.setRotation(btRot); // Aplica rota��o
    estadoDeMovimento->setWorldTransform(bt); // Aplica transforma��o
}

// Obter velocidade
glm::vec3 bubble::fisica::obterVelocidade() const
{
    return { corpoRigido->getLinearVelocity().getX(),
            corpoRigido->getLinearVelocity().getY(),
            corpoRigido->getLinearVelocity().getZ() };
}// Obter velocidade
glm::vec3 bubble::fisica::obterPosicao() const
{
    btTransform bt;
    corpoRigido->getMotionState()->getWorldTransform(bt);
    return { 
        bt.getOrigin().getX(),
        bt.getOrigin().getY(),
        bt.getOrigin().getZ()
    };
}