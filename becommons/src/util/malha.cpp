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
 * @file malha.cpp
 */

#include "glad.h"
#include "util/malha.hpp"
#include "util/material.hpp"
#include "nucleo/engine.hpp"
#include "depuracao/debug.hpp"
#include "util/vertice.hpp"
#include "sistemas/sistema_de_renderizacao.hpp"

using namespace becommons;


malha::malha(const std::vector<vertice>& vertices, const std::vector<unsigned int>& indices, const material& material) :
    m_vertices(vertices), m_indices(indices), m_material(material) {
}
         
std::vector<vertice>& malha::obterVertices() {
    return m_vertices;
}
        
std::vector<transformacao> malha::obterInstancias() const {
    return m_instancias;
}

std::vector<unsigned int>& malha::obterIndices() {
    return m_indices;
}
        
unsigned int malha::obterVertexArray() const {
    return m_VAO;
}
        
unsigned int malha::obterElementBuffer() const {
    return m_EBO;
}
        
unsigned int malha::obterVertexBuffer() const {
    return m_VBO;
}

void malha::definirVertices(const std::vector<vertice>& v) {
    m_vertices = v;
}

void malha::definirInstancias(const std::vector<transformacao>& i) {
    m_instancias = i;
}
        
void malha::definirIndices(const std::vector<unsigned int>& i) {
    m_indices = i;
}
        
void malha::definirSobreposicao(const bool s) {
    m_sobrepor = s;
}

void malha::descarregar() {
    if(!m_carregado) return;
    depuracao::emitir(debug, "malha", "descarregando");
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    m_carregado = false;
}

void malha::carregar() {
    if(m_carregado) return;
    
    if (m_instancias.empty()) {
        m_material.definirUniforme("instancia", false);
    } else {
        m_material.definirUniforme("instancia", true);
        for (size_t i = 0; i < m_instancias.size(); i++) {
            sistema_renderizacao::calcularTransformacao(&m_instancias[i]);
            m_material.definirUniforme("transformacoes[" + std::to_string(i) + "]", m_instancias[i].obterMatrizModelo());
        }
    }

    motor::obter().fila_opengl.push([&](){
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(vertice), &m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
        &m_indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertice), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertice), (void*)offsetof(vertice, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertice), (void*)offsetof(vertice, uvcoords));

    glBindVertexArray(0);
    m_carregado = true;
    });
}

void malha::desenhar(shader &shader) {
    m_material.usar(shader);
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}
        
bool malha::estaCarregado() const {
    return m_carregado;
}

bool malha::estaSobreposto() const {
    return m_sobrepor;
}

malha::~malha() {
    descarregar();
}
