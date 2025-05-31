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
 * @file bubble_gui.cpp
 */

#include "becommons_namespace.hpp"
#include "sistemas/bubble_gui.hpp"
#include "os/janela.hpp"
#include "depuracao/debug.hpp"
#include <cmath>
#include <functional>

using namespace BECOMMONS_NS;

void bubble_gui::atualizarLJ(caixa* it_caixa) {
    const bool is_horizontal = it_caixa->m_estilo.m_orientacao_modular == estilo::orientacao::horizontal;
    float mais_largo = 0.f;
    if(!deveAtualizar(it_caixa)) return;

    for(auto& filho : it_caixa->m_filhos)
    {
        if(!deveAtualizar(filho.get()) || filho->tem_flag(flag_estilo::largura_percentual)) continue;
        if(!is_horizontal && it_caixa->tem_flag(flag_estilo::largura_justa))
        {
        // Calcular largura justa
        mais_largo = std::max(mais_largo, filho->m_estilo.m_largura + filho->m_estilo.m_padding.x*(it_caixa->m_filhos.size()+1) + it_caixa->m_estilo.m_padding_geral.x*(it_caixa->m_filhos.size()+1));
        // Definir dimensão da caixa pai
        it_caixa->m_estilo.m_largura = mais_largo;
        }
    }
}
void bubble_gui::atualizarAJ(caixa* it_caixa) {
    const bool is_horizontal = it_caixa->m_estilo.m_orientacao_modular == estilo::orientacao::horizontal;
    if (!deveAtualizar(it_caixa)) return;

    float mais_alto = 0.f;
    float acumulado_altura = 0.f;
    size_t n_filhos = it_caixa->m_filhos.size();

    for (auto& filho : it_caixa->m_filhos) {
        if (!deveAtualizar(filho.get()) || filho->tem_flag(flag_estilo::altura_percentual))
            continue;

        // --- HORIZONTAL: altura justa é a maior altura entre os filhos (+ paddings) ---
        if (is_horizontal && it_caixa->tem_flag(flag_estilo::altura_justa)) {
            float height_com_padding =
                filho->m_estilo.m_altura +
                filho->m_estilo.m_padding.y * 2 +
                it_caixa->m_estilo.m_padding_geral.y * 2;
            mais_alto = std::max(mais_alto, height_com_padding);
            it_caixa->m_estilo.m_altura = mais_alto;
        }
        // --- VERTICAL: soma todas as alturas (+ paddings) para ter "altura justa" ---
        else if (!is_horizontal && it_caixa->tem_flag(flag_estilo::altura_justa)) {
            // Altura do filho + padding interno (top+bottom) + padding geral (top+bottom)
            float h = filho->m_estilo.m_altura
                    + filho->m_estilo.m_padding.y * 2
                    + it_caixa->m_estilo.m_padding_geral.y * 2;
            acumulado_altura += h;
        }
    }

    // Depois de somar todos os filhos, aplica à caixa pai (apenas no caso vertical)
    if (!is_horizontal && it_caixa->tem_flag(flag_estilo::altura_justa)) {
        it_caixa->m_estilo.m_altura = acumulado_altura;
    }
}

// Atualizar Hierarquia De Traz pra Frente
void bubble_gui::atualizarHDTF(caixa* it_caixa, std::function<void(caixa*)> func) {
    // Percorre os filhos de trás pra frente
    for (auto it = it_caixa->m_filhos.rbegin(); it != it_caixa->m_filhos.rend(); ++it) {
        // Chama recursivamente antes de atualizar a caixa atual
        atualizarHDTF(it->get(), func);
        
        func((*it).get());
    }
}

void bubble_gui::desenhar_caixa(caixa* c)
{
    if(!c->m_estilo.m_ativo) return;
    if(c->m_estilo.m_cor_fundo.a != 0) {
        renderizarFundo(c, quad_shader);
    }
    
    switch(c->tipo()) {
        case tipo_caixa::texto: {
            renderizarTexto(static_cast<elementos::texto*>(c));
            break;
        }
        case tipo_caixa::imagem: {
            auto img = static_cast<elementos::imagem*>(c);
            img->m_imagem_tamanho = { static_cast<int>(img->m_estilo.m_limites.z), static_cast<int>(img->m_estilo.m_limites.w) };
            renderizarImagem(img);
            break;
        }
        default:
            break;
    }

    for(auto& filho : c->m_filhos)
    {
        if(     filho->m_estilo.m_limites.x < c->m_estilo.m_limites.x + c->m_estilo.m_limites.z
                && filho->m_estilo.m_limites.y < c->m_estilo.m_limites.y + c->m_estilo.m_limites.w)
        desenhar_caixa(filho.get());
    }
}

void bubble_gui::renderizarImagem(elementos::imagem* img) const {
    glActiveTexture(GL_TEXTURE0);

    img->m_material.definirUniforme("projecao", proj);
    img->definirUniformesMaterial();
    img->m_material.usar(*img->m_imagem_shader);
    
    /*  desenho  */
    glBindVertexArray(ret_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void bubble_gui::renderizarFundo(caixa* it_caixa, BECOMMONS_NS::shader* quad_shader) const {
    quad_shader->use();
    quad_shader->setVec2("quadrado.posicao", it_caixa->m_estilo.m_limites.x, it_caixa->m_estilo.m_limites.y);
    quad_shader->setVec2("quadrado.tamanho", it_caixa->m_estilo.m_limites.z, it_caixa->m_estilo.m_limites.w);
    quad_shader->setVec2("resolucao_textura", it_caixa->m_estilo.m_limites.z, it_caixa->m_estilo.m_limites.w);
    quad_shader->setCor("cor_borda", it_caixa->m_estilo.m_cor_borda);
    quad_shader->setCor("cor", it_caixa->m_estilo.m_cor_fundo);
    quad_shader->setInt("tamanho_bordas", it_caixa->m_estilo.m_espessura_borda);
    quad_shader->setBool("mostrar_bordas", it_caixa->m_estilo.m_cor_borda.a != 0);
    quad_shader->setMat4("projecao", glm::value_ptr(proj));
        /*  desenho  */
    glBindVertexArray(ret_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void bubble_gui::renderizarTexto(elementos::texto* tex) const {

        // activate corresponding render state	
        tex->m_texto_shader->use();
        tex->m_texto_shader->setCor("textColor", tex->m_texto_cor);
        tex->m_texto_shader->setMat4("projecao", glm::value_ptr(proj));

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(text_VAO);

        // iterate through all characters
        std::string::const_iterator c;
        auto& chs = gerenciadorFontes::obterInstancia().obter(tex->m_texto_fonte, tex->m_texto_escala);
        float y_linha = tex->m_texto_escala;
        float x_linha = tex->m_estilo.m_limites.x; 
        if(((uint32_t)tex->m_texto_flags & (uint32_t)elementos::flags_texto::alinhamento_central)!=0) 
        {
            x_linha += tex->m_estilo.m_limites.z / 2 - tex->obterLargura(tex->m_texto_frase)/2;
        }
        if(tex->m_texto_frase_ptr) tex->m_texto_frase = *tex->m_texto_frase_ptr;
        
        std::string texto_final = tex->m_texto_frase;

        for(auto ca : texto_final)
        {
            if(ca == '\n') {y_linha += tex->m_texto_escala; x_linha = tex->m_estilo.m_limites.x; continue;}
            caractere ch;
            if (chs.empty())
                return;
            if(chs.find(ca) != chs.end())
                ch = chs.at(ca);
            else
                continue;
            
            float xpos = x_linha + ch.apoio.x;
            float ypos = tex->m_estilo.m_limites.y - ch.apoio.y + y_linha;

            if(y_linha > tex->m_estilo.m_limites.y + tex->m_estilo.m_limites.w) break;
            float w = ch.tamanho.x;
            float h = ch.tamanho.y;

            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 1.0f },
                { xpos,     ypos,       0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 0.0f },

                { xpos,     ypos + h,   0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 0.0f },
                { xpos + w, ypos + h,   1.0f, 1.0f }
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
            x_linha += (ch.avanco >> 6);
        }
        glBindVertexArray(0);
    }
bubble_gui::bubble_gui() {
}

void bubble_gui::inicializar() {
    depuracao::emitir(debug, "bubble_gui", "Inicializando sistema de GUI.");
    carregar();
}

bubble_gui::~bubble_gui() {
    if(quad_shader)
    delete quad_shader;
}

void bubble_gui::carregar()
{
    quad_shader = new shader("imagem.vert", "quad.frag");

    /*--------------BUFFERS--------------*/
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
        
        /*----------------quad---------------*/

        // definiçãoo de dados do quadrado (posição e UV)
        float vertices[] = {
            // posição (x, y)    // UV (u, v)
            0.0f, 0.0f,        0.0f, 0.0f, // Inferior esquerdo
             1.0f, 0.0f,        1.0f, 0.0f, // Inferior direito
             1.0f,  1.0f,        1.0f, 1.0f, // Superior direito
            0.0f,  1.0f,        0.0f, 1.0f  // Superior esquerdo
        };

        // indices para formar dois triângulos
        unsigned int indices[] = {
            0, 1, 2, // primeiro triângulo
            2, 3, 0  // segundo triângulo
        };

        // geração de VAO, VBO e EBO
        glGenVertexArrays(1, &ret_VAO);
        glGenBuffers(1, &ret_VBO);
        glGenBuffers(1, &ret_EBO);

        // configuração do VAO
        glBindVertexArray(ret_VAO);

        // configuração do VBO (dados do quadrado)
        glBindBuffer(GL_ARRAY_BUFFER, ret_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // configuração do EBO (indices)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // configuração do atributo de posição (layout location = 0)
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // configuração do atributo UV (layout location = 1)
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
}

void bubble_gui::adicionarFlags(const std::string& id, flag_estilo f) {
    if (auto caixa = obterElemento(id)) {
        caixa->m_estilo.m_flag_estilo |= f;
    }
}
bool bubble_gui::deveAtualizar(caixa* it_caixa) {
    if (!it_caixa->m_estilo.m_ativo) {
        return false; // Não atualiza caso a caixa esteja desativada 
    }

    if(it_caixa->m_estilo != it_caixa->m_estilo_antigo) {
        return true;
    }

    // Atualização forçada por evento de entrada
    if(janela::temInstancia() 
            && (janela::obterInstancia().m_inputs.m_estado_mouse == GLFW_PRESS 
                || janela::obterInstancia().m_inputs.m_estado_tecla == GLFW_PRESS)) {
        return true;
    }
    return false;
}

void bubble_gui::configOpenglState() const {
    glCullFace(GL_FRONT);
    glDisable(GL_DEPTH_TEST);
    glViewport(0, 0,
janela::obterInstancia().tamanho.x,
janela::obterInstancia().tamanho.y
            );
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);
}

void bubble_gui::deconfigOpenglState() const {
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
}

void bubble_gui::popFuncoes() {
    while(!funcoes.empty())
    {
        auto f = funcoes.front();
        f();
        funcoes.pop();
    }
}

void bubble_gui::atualizar()
{
    configOpenglState();

    janela::obterInstancia().defCursor(janela::cursor::seta);

    proj = glm::ortho(0.f, static_cast<float>(janela::obterTamanhoJanela().x), static_cast<float>(janela::obterTamanhoJanela().y), 0.f);
   
    popFuncoes();

    // atualiza dimensões da raiz
    if(!raiz) throw std::runtime_error("Raiz não definida!");
    raiz->m_estilo.m_limites = {0.f, 0.f,
        static_cast<float>(janela::obterTamanhoJanela().x),
        static_cast<float>(janela::obterTamanhoJanela().y)};

    // ajusta largura de último-primeiro
    atualizarHDTF(raiz.get(), atualizarLJ);
    // ajusta altura de último-primeiro
    atualizarHDTF(raiz.get(), atualizarAJ); 
    atualizarFilhos(raiz.get());
    
    desenhar_caixa(raiz.get());
    deconfigOpenglState();
}


void bubble_gui::organizarLinha(caixa* it_caixa,
                                bool is_horizontal,
                                const ivet2 range_filhos,
                                const fvet2& unidade_crescimento,
                                const fvet2& espaco_ocupado,
                                fvet2& cursor)
{
    float maior = 0.f; // o maior elemento
    // verificar se o pai tem a flag de alinhamento central
    if (it_caixa->tem_flag(flag_estilo::alinhamento_central)) {
        // Calcula o deslocamento inicial no eixo principal:
        // Se estiver no modo horizontal, desloca em X. Caso contrário, em Y.
        if (is_horizontal) {
            float larguraPai    = it_caixa->m_estilo.m_limites.z;   // largura total disponível
            // O início deve ser no meio do pai - largura filhos / 2
            cursor.x += (larguraPai - espaco_ocupado.x) / 2;
            // (cursor.y continua o mesmo aqui)
        }
        else {
            float alturaPai    = it_caixa->m_estilo.m_limites.w;    // altura total disponível
            // Desloca o cursor Y para que os filhos "comece(m)" no meio do espaço sobrando
            cursor.y += (alturaPai - espaco_ocupado.y) / 2;
            // (cursor.x continua o mesmo aqui)
        }
    } else if (it_caixa->tem_flag(flag_estilo::alinhamento_fim)) {
        if (is_horizontal) {
            float larguraPai    = it_caixa->m_estilo.m_limites.z;   // largura total disponível
            cursor.x += larguraPai - espaco_ocupado.x;
            // (cursor.y continua o mesmo aqui)
        }
        else {
            float alturaPai    = it_caixa->m_estilo.m_limites.w;    // altura total disponível
            cursor.y += alturaPai - espaco_ocupado.y;
            // (cursor.x continua o mesmo aqui)
        }
    }
    int i = range_filhos.x;
    while (i < range_filhos.y) {
        auto& filho = it_caixa->m_filhos[i];
        // atualiza dimenções
        filho->m_estilo.m_limites.z = filho->tem_flag(flag_estilo::largura_percentual) ? filho->m_estilo.m_largura * unidade_crescimento.x : filho->m_estilo.m_largura;
        filho->m_estilo.m_limites.w = filho->tem_flag(flag_estilo::altura_percentual) ? filho->m_estilo.m_altura * unidade_crescimento.y : filho->m_estilo.m_altura;

        filho->m_estilo.m_limites.x = cursor.x + filho->m_estilo.m_padding.x + it_caixa->m_estilo.m_padding_geral.x;
        filho->m_estilo.m_limites.y = cursor.y + filho->m_estilo.m_padding.y + it_caixa->m_estilo.m_padding_geral.y;
        if (is_horizontal) {
            maior = std::max(maior, filho->m_estilo.m_limites.w + filho->m_estilo.m_padding.y + it_caixa->m_estilo.m_padding_geral.y); // maior altura
            cursor.x += filho->m_estilo.m_limites.z + filho->m_estilo.m_padding.x + it_caixa->m_estilo.m_padding_geral.x;
        } else {
            maior = std::max(maior, filho->m_estilo.m_limites.z+ filho->m_estilo.m_padding.x + it_caixa->m_estilo.m_padding_geral.x); // maior largura
            cursor.y += filho->m_estilo.m_limites.w + filho->m_estilo.m_padding.y + it_caixa->m_estilo.m_padding_geral.y;
        }
        i++;
    }
    // quebra linha no final
    if(is_horizontal) {
        cursor.y += maior;
        cursor.x = it_caixa->m_estilo.m_limites.x;
    }
    else {
        cursor.x += maior;
        cursor.y = it_caixa->m_estilo.m_limites.y;
    }
}

void bubble_gui::processarDimensaoModular(caixa* filho, fvet2& crescimento_total, fvet2& espaco_ocupado) {
        if(filho->tem_flag(flag_estilo::largura_percentual)) 
            if(filho->m_pai && filho->m_pai->m_estilo.m_orientacao_modular == estilo::orientacao::horizontal)
                crescimento_total.x += filho->m_estilo.m_largura;
            else
                crescimento_total.x = std::max(crescimento_total.x, filho->m_estilo.m_largura);
        else 
            espaco_ocupado.x += filho->m_estilo.m_largura + filho->m_estilo.m_padding.x;
        
        if(filho->tem_flag(flag_estilo::altura_percentual))
            if(filho->m_pai && filho->m_pai->m_estilo.m_orientacao_modular == estilo::orientacao::vertical)
                crescimento_total.y += filho->m_estilo.m_altura;
            else
                crescimento_total.y = std::max(crescimento_total.y, filho->m_estilo.m_altura);
        else 
            espaco_ocupado.y += filho->m_estilo.m_altura + filho->m_estilo.m_padding.y;
}

void bubble_gui::processarModular(caixa* it_caixa) {
    // se a caixa for modular, atualizar limites dos filhos
    if(!it_caixa->tem_flag(flag_estilo::modular)) return;

    const bool is_horizontal = it_caixa->m_estilo.m_orientacao_modular == estilo::orientacao::horizontal;

    fvet2 espaco_disponivel = fvet2(it_caixa->m_estilo.m_limites.z, it_caixa->m_estilo.m_limites.w), // espaço disponível para redimensionamento de filhos
          espaco_ocupado = fvet2(0.f, 0.f),           // espaço ocupado pelos filhos
          cursor            = fvet2(it_caixa->m_estilo.m_limites.x, it_caixa->m_estilo.m_limites.y),
          crescimento_total = fvet2(0.f, 0.f);  // total ocupado pelo crescimento dos filhos.
   
    // calcula espaços e aplica dimenções
    ivet2 range_filhos = ivet2(0, 0);
    for(size_t i = 0; i < it_caixa->m_filhos.size(); i++) {
        auto& filho = it_caixa->m_filhos[i];
        if(!filho->m_estilo.m_ativo) continue;

        espaco_ocupado.x += it_caixa->m_estilo.m_padding_geral.x;
        espaco_ocupado.y += it_caixa->m_estilo.m_padding_geral.y;
    
        bool finalizar_linha = (i == it_caixa->m_filhos.size() - 1) || (filho->tem_flag(flag_estilo::quebrar_linha));
        if(finalizar_linha) {
            espaco_ocupado.x += it_caixa->m_estilo.m_padding_geral.x;
            espaco_ocupado.y += it_caixa->m_estilo.m_padding_geral.y;
        }

        processarDimensaoModular(filho.get(),
                crescimento_total,
                espaco_ocupado
                );
        // continua iterando até finalizar linha...
        if(finalizar_linha) {
            range_filhos.x = range_filhos.y;
            range_filhos.y = i + 1;
            // calcula dimenções ao finalizar a linha.
            // \{
            espaco_disponivel -= espaco_ocupado;
            espaco_disponivel.x = std::max(espaco_disponivel.x, 0.f); // limita a no máximo 0.
            espaco_disponivel.y = std::max(espaco_disponivel.y, 0.f); // limita a no máximo 0.
            // a unidade de crescimento é a parcela de cade filho com crescimento.
            // seu crescimento será multiplicado pelo unidade, e ocupará o espaço disponível.
            fvet2 unidade_crescimento = espaco_disponivel / crescimento_total;

            // \}
            // itera a linha
            // \{
            organizarLinha(it_caixa,    // pai
                    is_horizontal,
                    range_filhos,
                    unidade_crescimento,
                    espaco_ocupado,
                    cursor
                    );

            // \}
            // reseta a linha/coluna atual e continua até acabar os filhos.
            // \{
            if(is_horizontal) {
                espaco_disponivel.x = it_caixa->m_estilo.m_limites.z;
                espaco_ocupado.x = 0;
                crescimento_total.x = 0;
            }
            else {
                espaco_disponivel.y = it_caixa->m_estilo.m_limites.w;
                espaco_ocupado.y = 0;
                crescimento_total.y = 0;
            }    
            // \}
        }
    }
}

void bubble_gui::atualizarFilhos(caixa* it_caixa) {
    if (!it_caixa) {
        throw std::runtime_error("Caixa nula sendo atualizada.");
    }

    switch (it_caixa->tipo()) {
    case tipo_caixa::caixa_de_texto: {
        auto ct = static_cast<elementos::caixa_de_texto*>(it_caixa);
        ct->atualizar(); 
        break;
                                     }
    case tipo_caixa::botao: {
        auto btn = static_cast<elementos::botao*>(it_caixa);
        if(btn->pressionado() && btn->m_use_funcao)
            funcoes.push(btn->m_funcao);
        break;
                            }
    }
    if(deveAtualizar(it_caixa)) {
        processarModular(it_caixa);
    }
    it_caixa->m_estilo_antigo = it_caixa->m_estilo;
    // Atualiza recursivamente os filhos
    for (auto& filho : it_caixa->m_filhos) {
        atualizarFilhos(filho.get());
    }
}
void bubble_gui::removerElemento(const std::string& id) {
    if(caixas.find(id) == caixas.end()) return;
    for(auto& filho : caixas[id]->m_filhos)
    {
        caixas.erase(filho->m_id);
    }
    caixas[id]->m_filhos.clear();
    caixas.erase(id);
}
void bubble_gui::removerFilhos(const std::string& id)
{
    if(caixas.find(id) == caixas.end()) return;
    for(auto& filho : caixas[id]->m_filhos)
    {
        caixas.erase(filho->m_id);
    }
    caixas[id]->m_filhos.clear();
}

caixa* bubble_gui::obterElemento(const std::string& id) {
    // Caso não tenha terminado o estilo atual
    if(m_novo_estilo) {
        throw std::runtime_error("estilo atual não finalizado!");
    }
    auto it = caixas.find(id);
    if (it != caixas.end()) {
        return it->second;
    }
    // Isso também pode acontecer ao adicionar um filho de forma direta pelo ponteiro da caixa.
    // Use a função "adicionar" invés disso.
    throw std::runtime_error("elemento não encontrado!");
}
bool bubble_gui::elementoExiste(const std::string id) const {
    auto it = caixas.find(id);
    return it != caixas.end();
}

// Funções de estilo
void bubble_gui::iniciarRaiz(const std::string& nome) {
    raiz = std::make_unique<caixa>();
    raiz->m_id = nome;
    estilo_atual[nome] = raiz.get();
    m_novo_estilo = true;
}
void bubble_gui::fimEstilo(){
    for(auto& [id, caixa] : estilo_atual) {
        caixa->configurar();
        caixas[id] = caixa;
    }
    m_novo_estilo = false;
    estilo_atual.clear();
};
void bubble_gui::defFlags(const flag_estilo v){
    for(auto& [id, caixa] : estilo_atual){
        caixa->m_estilo.m_flag_estilo = v;
    }
}
void bubble_gui::defLargura(const double v){
    for(auto& [id, caixa] : estilo_atual){
        caixa->m_estilo.m_largura = v;
        caixa->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual;
    }
}
void bubble_gui::defLarguraAltura(const bool b){
    for(auto& [id, caixa] : estilo_atual){
        caixa->m_estilo.m_ligar_la = b;
    }
}
void bubble_gui::defAtivo(const bool b){
    for(auto& [id, caixa] : estilo_atual){
        caixa->m_estilo.m_ativo = b;
    }
}
void bubble_gui::defAltura(const double v){
    for(auto& [id, caixa] : estilo_atual){
        caixa->m_estilo.m_altura = v;
        caixa->m_estilo.m_flag_estilo |= flag_estilo::altura_percentual;
    }
}
void bubble_gui::defLargura(const int v){
    for(auto& [id, caixa] : estilo_atual){
        caixa->m_estilo.m_largura = v;
    }
}
void bubble_gui::defAltura(const int v){
    for(auto& [id, caixa] : estilo_atual){
        caixa->m_estilo.m_altura = v;
    }
}
void bubble_gui::defOrientacao(const estilo::orientacao v){
    for(auto& [id, caixa] : estilo_atual){
        caixa->m_estilo.m_orientacao_modular = v;
    }
}
void bubble_gui::defPaddingG(const int v1, const int v2){
    for(auto& [id, caixa] : estilo_atual){
        caixa->m_estilo.m_padding_geral = {v1, v2};
    }
}
void bubble_gui::defPadding(const int v1, const int v2){
    for(auto& [id, caixa] : estilo_atual){
        caixa->m_estilo.m_padding = {v1, v2};
    }
}
void bubble_gui::defCorBorda(const cor v){
    for(auto& [id, caixa] : estilo_atual){
        caixa->m_estilo.m_cor_borda = v;
    }
}
void bubble_gui::defCorFundo(const cor v){
    for(auto& [id, caixa] : estilo_atual){
        caixa->m_estilo.m_cor_fundo = v;
    }
}
void bubble_gui::defCrescimentoM(const float v){
    for(auto& [id, caixa] : estilo_atual){
        if(caixa->m_estilo.m_orientacao_modular == estilo::orientacao::horizontal)
            defLargura(v);
        else
            defAltura(v);
    }
}
void bubble_gui::defTamanhoBorda(const int v){
    for(auto& [id, caixa] : estilo_atual){
        caixa->m_estilo.m_espessura_borda = v;
    }
}
