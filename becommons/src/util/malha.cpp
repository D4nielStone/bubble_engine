#include <glad/glad.h>
#include "util/malha.hpp"
#include "util/material.hpp"
#include "depuracao/debug.hpp"

namespace bubble
{
    malha::malha(std::vector<vertice>& vertices, std::vector<unsigned int>& indices, bubble::material& material)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->material = material;

        definirBuffers();
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
    void malha::desenhar(bubble::shader &shader)
    {
        if(sobrepor)
            glDepthFunc(GL_ALWAYS);
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;

        shader.setFloat("material.brilho", material.brilho);
        shader.setCor("material.cor_especular", material.especular);
        shader.setCor("material.cor_difusa", material.difusa);
        shader.setBool("uvMundo", material.uvMundo);
        for (unsigned int i = 0; i < material.texturas.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string name = material.texturas[i].tipo;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);

            shader.setInt((name + number).c_str(), i);
            shader.setBool((name + number + "_bool").c_str(), true);
            glBindTexture(GL_TEXTURE_2D, material.texturas[i].id);
        }
        shader.setBool("recebe_luz", material.recebe_luz);
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

        diffuseNr = 1;
        specularNr = 1;
        //desativa texturas
        for (unsigned int i = 0; i < material.texturas.size(); i++)
        {
            std::string number;
            std::string name = material.texturas[i].tipo;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);
            shader.setBool((name + number + "_bool").c_str(), false);
        }
        if(sobrepor)
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
