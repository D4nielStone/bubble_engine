/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file raio.hpp
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#pragma once
#include <glm/glm.hpp>
#include <bullet/btBulletDynamicsCommon.h>
#include "becommons.hpp"

namespace BECOMMONS_NS
{
    // Estrutura para armazenar os resultados do Raycast
    struct raio
    {
        glm::vec3 origem;
        glm::vec3 direcao;
    };
    struct resultadoRaio {
        bool atingiu;                      // Se o raio atingiu algo
        glm::vec3 pontoDeColisao;            // Ponto de colis�o
        glm::vec3 normalAtingida;           // Normal da superf�cie atingida
        const btCollisionObject* objetoAtingido; // Objeto atingido
    };
}
