/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file malha.hpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#pragma once
#include "vertice.hpp"
#include <vector>
#include "textura.hpp"
#include "arquivadores/shader.hpp"
#include "util/material.hpp"

namespace bubble
{
    class malha
    {
    public:
        bool sobrepor = false;
        std::vector<glm::vec3> instancias_pos;
        std::vector<vertice> vertices;
        std::vector<unsigned int> indices;
        bubble::material material;

        malha() = default;
        malha(std::vector<vertice>& vertices, std::vector<unsigned int> &indices, bubble::material& material);
        ~malha();
        void descarregar();
        void desenhar(bubble::shader& shader);
        void definirBuffers();
        //  dados de renderizacao
        unsigned int VAO, VBO, EBO;

    };
}
