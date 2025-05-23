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
 * @file fisica.hpp
 * @brief Componente de física para entidades ECS.
 *
 * @see fisica.cpp
 */

#pragma once
#include <glm/glm.hpp>
#include "becommons_namespace.hpp"
#include "util/malha.hpp"
#include "util/vetor3.hpp"
#include "componente.hpp"
#include "transformacao.hpp"

namespace BECOMMONS_NS {
    struct fisica : componente {
        static constexpr mascara mascara { COMPONENTE_FISICA };
        enum camada {
            COLISAO_PADRAO = 1 << 0,  // Objetos comuns
            COLISAO_ESPECIAL = 1 << 1 // Objeto especial
        };

        camada camada_colisao{camada::COLISAO_PADRAO};
        fisica() = default;
        fisica(btCollisionShape* forma, btScalar massa, btVector3 posicaoInicial, camada camada = COLISAO_PADRAO);
        fisica(bool usar_malha, btScalar massa, btVector3 posicaoInicial, camada camada = fisica::COLISAO_PADRAO);
        virtual ~fisica();

        btRigidBody* obterCorpoRigido();
        void criarMalha();
        void atualizarTransformacao();
        void aplicarForca(const fvet3& vetor);
        void aplicarVelocidade(const fvet3& vetor);
        void definirPosicao(const fvet3& posicao); 
        void definirFatorLinear(const fvet3& fator);
        void definirFatorAngular(const fvet3& fator);
        void definirFriccao(const float fator);
        void definirRestituicao(const float fator);
        void definirRaioCcd(const float fator);
       void definirRotacao(const fvet3& rotacao);
        fvet3 obterVelocidade() const;
        fvet3 obterPosicao() const;
        void init();
        btScalar massa;

    private:
        bool usar_malha{ false };
        btCollisionShape* forma {nullptr};
        btDefaultMotionState* estadoDeMovimento{nullptr};
        btRigidBody* corpoRigido{ nullptr };
        btVector3 posicaoInicial;
        transformacao* m_transformacao {nullptr};
    };
}
