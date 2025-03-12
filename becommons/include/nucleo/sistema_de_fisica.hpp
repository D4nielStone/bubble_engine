/** @copyright Copyright (c) 2025 Daniel Oliveira */

/**
 * @file sistema_de_fisica.hpp
 * @brief Gerencia a fisica de jogo com o bullet3
 */

#pragma once
#include "sistema.hpp"
#include "util/raio.hpp"
#include <bullet/btBulletDynamicsCommon.h>
#include <thread>
#include <atomic>

namespace bubble
{
    inline static btDiscreteDynamicsWorld* mundoDinamicoPrincipal;
    
    class sistema_fisica : public sistema {
    public:
         sistema_fisica();
        ~sistema_fisica();

        void atualizar() override;
        void inicializar(bubble::fase* f) override;
        void iniciarThread();
        void pararThread();
        void remover(btRigidBody*& corpo);
        btDiscreteDynamicsWorld* mundo();

    private:
        std::atomic<bool> rodando{ false }; // Controle da thread
        std::thread fisicaThread;

        btDefaultCollisionConfiguration* configColisao{nullptr};
        btCollisionDispatcher* expedidor{nullptr};
        btBroadphaseInterface* faseAmpla{nullptr};
        btSequentialImpulseConstraintSolver* solucionador{nullptr};
        btDiscreteDynamicsWorld* mundoDinamico{nullptr};
    };

    // Fun��o de Raycast
    resultadoRaio raioIntersecta(const raio& raio);
}