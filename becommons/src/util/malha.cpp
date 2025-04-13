
/** @copyright 
MIT LicenseCopyright (c) 2025 Daniel Oliveira

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
 * @file malha.cpp
 */

#include "namespace.hpp"
#include <glad/glad.h>
#include "util/malha.hpp"
#include "util/material.hpp"
#include "depuracao/debug.hpp"
#include "util/vertice.hpp"

namespace BECOMMONS_NS {
    malha::malha(std::vector<vertice>& vertices, std::vector<unsigned int>& indices, material& material)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->m_material = material;
    }

    malha::~malha()
    {
        
    }

    void malha::descarregar()
    {
        depuracao::emitir(debug, "malha", "descarregando");
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
    void malha::desenhar(shader &shader)
    {
        if(m_sobrepor)
            glDepthFunc(GL_ALWAYS);

        shader.setBool("recebe_luz", m_material.recebe_luz);
        shader.setCor("material.albedo", m_material.albedo);
        shader.setFloat("material.metallic", m_material.metallic);
        shader.setFloat("material.ao", m_material.ao);
        shader.setFloat("material.roughness", m_material.roughness);
        shader.setBool("uvMundo", m_material.uvMundo);

        // texturas
        size_t i = 0;
        for (auto &textura : m_material.texturas)
        {
            if(textura.second.id == 0) continue;
            glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding

            std::string name = textura.first;
            shader.setInt(name.c_str(), i);
            shader.setBool(("use_" + name).c_str(), true);

            glBindTexture(GL_TEXTURE_2D, textura.second.id);
            i++;
        }
        glActiveTexture(GL_TEXTURE0);

        glBindVertexArray(VAO);
        if (instancias_pos.empty())
        {
            shader.setBool("instancia", false);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        }
        else
        {
            shader.setBool("instancia", true);
            for (size_t i = 0; i < instancias_pos.size(); i++)
            {
                shader.setVec3("posicoes[" + std::to_string(i) + "]", instancias_pos[i].x, instancias_pos[i].y, instancias_pos[i].z);
            }
            glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, instancias_pos.size());
        }
        glBindVertexArray(0);

        //desativa texturas
        for (auto &textura : m_material.texturas)
        {
            shader.setBool(("use_" + textura.first).c_str(), false);
        }

        if(m_sobrepor)
            glDepthFunc(GL_LESS);
    }

    void malha::definirBuffers()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertice), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
            &indices[0], GL_STATIC_DRAW);

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
    }
}
