/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file malha*.hpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "namespace.hpp"
#include "material.hpp"
#include "vertice.hpp"
#include "arquivadores/shader.hpp"

namespace BECOMMONS_NS {
    class malha
    {
        public:
        malha() = default;
        bool m_sobrepor = false;
        std::vector<glm::vec3> instancias_pos;
        std::vector<vertice> vertices;
        std::vector<unsigned int> indices;
        material m_material;
        malha(std::vector<vertice>& vertices, std::vector<unsigned int> &indices, material& material);
        ~malha();
        void descarregar();
        void desenhar(shader& shader);
        void definirBuffers();
        //  dados de renderizacao
        unsigned int VAO, VBO, EBO;

    };
}
