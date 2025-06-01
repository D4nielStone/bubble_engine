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
 */
/**
 * @file malha.hpp
 */

#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "becommons_namespace.hpp"
#include "material.hpp"
#include "vertice.hpp"
#include "componentes/transformacao.hpp"
#include "arquivadores/shader.hpp"

namespace BECOMMONS_NS {
    /**
     * \class malha
     * \brief Gerencia os vertices, indices, material e ect.
     * Usada para renderizar objetos 3D.
     */
     class malha {
     public:
        malha() = default;
        malha(const std::vector<vertice>& vertices, const std::vector<unsigned int> &indices, const material& material);
        std::vector<vertice>        obterVertices() const;
        std::vector<transformacao>  obterInstancias() const;
        std::vector<unsigned int>   obterIndices() const;
        material                    obterMaterial() const;
        unsigned int                obterVertexArray() const;
        unsigned int                obterElementBuffer() const;
        unsigned int                obterVertexBuffer() const;
        void                        definirVertices(const std::vector<vertice>&);
        void                        definirInstancias(const std::vector<transformacao>&);
        void                        definirIndices(const std::vector<unsigned int>&);
        void                        definirMaterial(const material&);
        void                        definirSobreposicao(const bool);
        void                        descarregar();
        void                        carregar();
        void                        desenhar(shader& shader);
        bool                        estaCarregado() const;
        bool                        estaSobreposto() const;
        ~malha();
     private:
        bool                        m_sobrepor { false }, 
                                    m_carregado { false };
        std::vector<transformacao>  m_instancias;
        std::vector<vertice>        m_vertices;
        std::vector<unsigned int>   m_indices;
        material                    m_material;
        unsigned int                m_VAO,
                                    m_VBO,
                                    m_EBO;

    };
} // namespace becommons
