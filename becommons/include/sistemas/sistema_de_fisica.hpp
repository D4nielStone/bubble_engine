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
 * @file sistema_de_fisica.hpp
 */

#pragma once
#include "sistema.hpp"
#include <iostream>
#include "util/raio.hpp"
#include <bullet/btBulletDynamicsCommon.h>

class MyContactCallback : public btCollisionWorld::ContactResultCallback {
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

namespace becommons {
    enum CollisionLayers {
        LAYER_GAME = 1 << 0,
        LAYER_EDITOR = 1 << 1,
        LAYER_PICKING = 1 << 2
    };
    struct sistema_fisica : sistema {
         sistema_fisica();
        ~sistema_fisica();

        void atualizar() override;
        void atualizarColisoes();
        void inicializar() override;
        bool remover(btRigidBody*& corpo);
        btDiscreteDynamicsWorld* mundo();
        int m_depth = 10;
        btVector3 m_gravity = btVector3(0.f, 9.8f, 0.f);
        resultadoRaio emitirRaio(const raio& raio);
    private:
        btDefaultCollisionConfiguration* configColisao{nullptr};
        btCollisionDispatcher* expedidor{nullptr};
        btBroadphaseInterface* faseAmpla{nullptr};
        btSequentialImpulseConstraintSolver* solucionador{nullptr};
        btDiscreteDynamicsWorld* mundoDinamico{nullptr};
    };

    // Função de Raycast
}
