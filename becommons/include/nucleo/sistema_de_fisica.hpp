/** @copyright Copyright (c) 2025 Daniel Oliveira */

/**
 * @file sistema_de_fisica.hpp
 * @brief Gerencia a fisica de jogo com o bullet3
 */

#pragma once
#include "sistema.hpp"
#include <iostream>
#include "util/raio.hpp"
#include <bullet/btBulletDynamicsCommon.h>
#include <thread>
#include <atomic>

class MyContactCallback : public btCollisionWorld::ContactResultCallback
{
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
        bool remover(btRigidBody*& corpo);
        btDiscreteDynamicsWorld* mundo();
        inline static float velocidade = 1.f;
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
