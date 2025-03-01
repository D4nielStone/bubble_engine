/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
#include <glm/glm.hpp>
#include <bullet/btBulletDynamicsCommon.h>

namespace bubble
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