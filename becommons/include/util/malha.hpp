/** @copyright Copyright (c) 2025 Daniel Oliveira */
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
    private:
        //  dados de renderizacao
        unsigned int VAO, VBO, EBO;

    };
}