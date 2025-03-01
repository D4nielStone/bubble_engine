#include <glad/glad.h>
#include "nucleo/sistema_de_interface.hpp"
#include "componentes/renderizador.hpp"
#include "componentes/transformacao.hpp"
#include "componentes/camera.hpp"
#include "nucleo/fase.hpp"
#include "arquivadores/shader.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "componentes/texto.hpp"
#include "arquivadores/fonte.hpp"
#include "os/janela.hpp"

bubble::shader* shader_texto{ nullptr };
bubble::shader* shader_imagem{ nullptr };
glm::mat4 projection{1.f};

namespace bubble
{
    sistemaInterface::~sistemaInterface()
    {
        glDeleteBuffers(1, &text_VBO);
        glDeleteBuffers(1, &img_VBO);
        glDeleteVertexArrays(1, &text_VAO);
        glDeleteVertexArrays(1, &img_VAO);
    }
    void sistemaInterface::atualizar()
    {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        projection = glm::ortho(0.0, instanciaJanela->tamanho.x, 0.0, instanciaJanela->tamanho.y);
        reg->cada<bubble::texto>([&](const uint32_t ent) 
            {
                desenharTexto(*shader_texto, *reg->obter<bubble::texto>(ent));
            }
        );
        reg->cada<bubble::imagem>([&](const uint32_t ent) 
            {
                desenharImagem(*shader_imagem, *reg->obter<bubble::imagem>(ent));
            }
        );
    }

    void sistemaInterface::inicializar(bubble::fase* fase_ptr)
    {
        this->_Mfase = fase_ptr;
        this->reg = _Mfase->obterRegistro();

        if (!shader_texto) shader_texto = new bubble::shader("texto.vert", "texto.frag");
        if (!shader_imagem) shader_imagem = new bubble::shader("imagem.vert", "imagem.frag");
        /*---------------texto---------------*/
        glGenVertexArrays(1, &text_VAO);
        glGenBuffers(1, &text_VBO);
        glBindVertexArray(text_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, text_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        /*--------------imagem--------------*/
        // Defini��o de dados do quadrado (posi��o e UV)
        float vertices[] = {
            // Posi��o (x, y)    // UV (u, v)
            0.0f, 0.0f,        0.0f, 0.0f, // Inferior esquerdo
             1.0f, 0.0f,        1.0f, 0.0f, // Inferior direito
             1.0f,  1.0f,        1.0f, 1.0f, // Superior direito
            0.0f,  1.0f,        0.0f, 1.0f  // Superior esquerdo
        };

        // �ndices para formar dois tri�ngulos
        unsigned int indices[] = {
            0, 1, 2, // Primeiro tri�ngulo
            2, 3, 0  // Segundo tri�ngulo
        };

        // Gera��o de VAO, VBO e EBO
        glGenVertexArrays(1, &img_VAO);
        glGenBuffers(1, &img_VBO);
        glGenBuffers(1, &img_EBO);

        // Configura��o do VAO
        glBindVertexArray(img_VAO);

        // Configura��o do VBO (dados do quadrado)
        glBindBuffer(GL_ARRAY_BUFFER, img_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Configura��o do EBO (�ndices)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, img_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Configura��o do atributo de posi��o (layout location = 0)
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Configura��o do atributo UV (layout location = 1)
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Desassociar o VAO para evitar altera��es
        glBindVertexArray(0);

    }

    void bubble::sistemaInterface::desenharTexto(shader& s, const bubble::texto &_texto)
    {
        auto text = _texto;
        // activate corresponding render state	
        s.use();
        s.setCor("textColor", text.cor);
        s.setMat4("projecao", glm::value_ptr(projection));

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(text_VAO);

        // iterate through all characters
        std::string::const_iterator c;
        auto& chs = bubble::gerenciadorFontes::obterInstancia().obter(text.fonte);
        for(char32_t ca : text.frase)
        {
            if (chs.empty())
                return;
            bubble::caractere ch = chs.at(ca);
            
            float xpos = text.padding.x + ch.apoio.x * text.escala;
            float ypos = text.padding.y - (ch.tamanho.y - ch.apoio.y) * text.escala;

            float w = ch.tamanho.x * text.escala;
            float h = ch.tamanho.y * text.escala;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.id);
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, text_VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            text.padding.x += (ch.avanco >> 6) * text.escala; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void bubble::sistemaInterface::desenharImagem(shader& s, const bubble::imagem& img)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, img.id);
        s.use();
        s.setVec2("quadrado.posicao", img.padding.x,img.padding.y);
        s.setVec2("quadrado.tamanho", img.limite.x, img.limite.y);
        s.setCor("difusa", { img.difusa });
        s.setInt("textura", 0);
        s.setInt("flip", img.flip);
        s.setMat4("projecao", glm::value_ptr(projection));
        /*  desenho  */
        glBindVertexArray(img_VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}