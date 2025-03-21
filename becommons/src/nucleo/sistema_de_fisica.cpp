#include "nucleo/sistema_de_fisica.hpp"
#include "nucleo/fase.hpp"
#include "os/janela.hpp"
#include "componentes/transformacao.hpp"
#include "componentes/fisica.hpp"

bubble::sistema_fisica::sistema_fisica()
{
    configColisao = new btDefaultCollisionConfiguration();
    expedidor = new btCollisionDispatcher(configColisao);
    faseAmpla = new btDbvtBroadphase();
    solucionador = new btSequentialImpulseConstraintSolver();
    mundoDinamico = new btDiscreteDynamicsWorld(expedidor, faseAmpla, solucionador, configColisao);
    mundoDinamico->setGravity(btVector3(0, -9.8, 0));
    mundoDinamico->getSolverInfo().m_numIterations = 100;

}
bubble::sistema_fisica::~sistema_fisica()
{
    pararThread();

    // Libere os outros componentes
    if(solucionador)delete solucionador;
    if(faseAmpla)delete faseAmpla;
    if(expedidor)delete expedidor;
    if(configColisao)delete configColisao;
    if(mundoDinamico)delete mundoDinamico;
}


void bubble::sistema_fisica::atualizar()
{
    mundoDinamico->stepSimulation(instanciaJanela->_Mtempo.obterDeltaTime() * velocidade, 1, 1.f / 1000.f);
    reg->cada<bubble::fisica, bubble::transformacao>([&](const uint32_t entidade)
        {
            /// adiciona corpos rigidos
            reg->obter<fisica>(entidade)->atualizarTransformacao();
        }
    );
}

void bubble::sistema_fisica::inicializar(bubble::fase* f)
{
    sistema_fisica::sistema::inicializar(f);
    mundoDinamicoPrincipal = mundoDinamico;
    reg->cada<bubble::fisica, bubble::transformacao>([&](const uint32_t entidade)
        {
            /// adiciona corpos rigidos
            auto comp_fisica = reg->obter<fisica>(entidade);
            // Adicionar ao mundo com grupo e m�scara
            int camada = comp_fisica->camada_colisao;
            int mascara = comp_fisica->camada_colisao;
            mundoDinamicoPrincipal->addRigidBody(comp_fisica->obterCorpoRigido(), camada, mascara);
        }
    );
}

void bubble::sistema_fisica::iniciarThread()
{
    rodando = true;
    fisicaThread = std::thread([this]() {
        while (rodando) {
            {
                this->atualizar();
            }
        }
        });
}

void bubble::sistema_fisica::pararThread()
{
    rodando = false;
    if (fisicaThread.joinable()) {
        fisicaThread.join();
    }
}

bool bubble::sistema_fisica::remover(btRigidBody*& corpo)
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

btDiscreteDynamicsWorld* bubble::sistema_fisica::mundo()
{
    return mundoDinamico;
}

bubble::resultadoRaio bubble::raioIntersecta(const raio& raio)
{
    // Configura��o do ponto inicial e final do raio no espa�o 3D
    btVector3 origem(raio.origem.x, raio.origem.y, raio.origem.z);
    btVector3 destino = origem + btVector3(raio.direcao.x, raio.direcao.y, raio.direcao.z) * 300.0f;

    // Criar o callback com m�scaras de camada
    btCollisionWorld::ClosestRayResultCallback callback(origem, destino);
    callback.m_collisionFilterGroup = fisica::COLISAO_PADRAO | fisica::COLISAO_ESPECIAL; // Raycast pode detectar ambas
    callback.m_collisionFilterMask = fisica::COLISAO_ESPECIAL;                  // Detectar apenas o especial

    // Executar o raycast
    mundoDinamicoPrincipal->rayTest(origem, destino, callback);

    // Estrutura de retorno
    resultadoRaio resultado = { false, glm::vec3(0.0f), glm::vec3(0.0f), nullptr };

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
