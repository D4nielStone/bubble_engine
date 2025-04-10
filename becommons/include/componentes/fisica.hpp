/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file fisica.hpp
 * @brief Componente de física para entidades ECS.
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 *
 * @see fisica.cpp
 */

#pragma once
#include <bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include "namespace.hpp"
#include "util/malha.hpp"
#include "componente.hpp"

namespace BECOMMONS_NS
{
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
        void aplicarForca(const glm::vec3& vetor);
        void aplicarVelocidade(const glm::vec3& vetor);
        void definirPosicao(const glm::vec3& posicao); 
        void definirFatorLinear(const glm::vec3& fator);
        void definirFatorAngular(const glm::vec3& fator);
        void definirFriccao(const float fator);
        void definirRestituicao(const float fator);
        void definirRaioCcd(const float fator);
       void definirRotacao(const glm::vec3& rotacao);
        glm::vec3 obterVelocidade() const;
        glm::vec3 obterPosicao() const;
        void init();
        btScalar massa;

    private:
        bool usar_malha{ false };
        btCollisionShape* forma;
        btDefaultMotionState* estadoDeMovimento;
        btRigidBody* corpoRigido{ nullptr };
        btVector3 posicaoInicial;
        transformacao* m_transformacao;
    };
}
