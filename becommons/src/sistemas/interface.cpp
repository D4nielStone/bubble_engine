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
 * @file interface.cpp
 */

#include "sistemas/interface.hpp"
#include "os/janela.hpp"
#include "nucleo/engine.hpp"
#include "depuracao/debug.hpp"
#include "elementos/popup.hpp"
#include <cmath>
#include <functional>

using namespace becommons;

void interface::atualizarLJ(caixa* it_caixa) {
    if(!it_caixa) return;
    const bool is_horizontal = it_caixa->m_estilo.m_orientacao_modular == estilo::orientacao::horizontal;
    if (!deveAtualizar(it_caixa)) return;

    float mais_largo = 10.f;
    float acumulado_largura = 0.f;
    size_t n_filhos = it_caixa->m_filhos.size();

    for (auto& filho : it_caixa->m_filhos) {
        if (!deveAtualizar(filho.get()) || filho->tipo() == tipo_caixa::popup || filho->tem(flag_estilo::altura_percentual))
            continue;
        // --- VERTICAL: largura justa é a maior largura entre os filhos (+ paddings) ---
        if (!is_horizontal && it_caixa->tem(flag_estilo::largura_justa)) {
            float width_com_padding =
                filho->m_estilo.m_largura +
                filho->m_estilo.m_padding.x * 2 +
                it_caixa->m_estilo.m_padding_geral.x * 2;
            mais_largo = std::max(mais_largo, width_com_padding);
        }
        // --- HORIZONTAL: soma todas as larguras (+ paddings) para ter "largura justa" ---
        else if (is_horizontal && it_caixa->tem(flag_estilo::largura_justa)) {
            float w = filho->m_estilo.m_largura
                    + filho->m_estilo.m_padding.x * 2
                    + it_caixa->m_estilo.m_padding_geral.x * 2;
            acumulado_largura += w;
        }
    }

    // Aplica a largura final à caixa pai
    if (!is_horizontal && it_caixa->tem(flag_estilo::largura_justa)) {
        it_caixa->m_estilo.m_largura = mais_largo;
    }
    else if (is_horizontal && it_caixa->tem(flag_estilo::largura_justa)) {
        it_caixa->m_estilo.m_largura = acumulado_largura;
    }
}
void interface::atualizarAJ(caixa* it_caixa) {
    if(!it_caixa) return;
    const bool is_horizontal = it_caixa->m_estilo.m_orientacao_modular == estilo::orientacao::horizontal;
    if (!deveAtualizar(it_caixa)) return;

    float mais_alto = 10.f;
    float acumulado_altura = 0.f;
    size_t n_filhos = it_caixa->m_filhos.size();
    bool quebra_linha = false;

    for (auto& filho : it_caixa->m_filhos) {
        if (!deveAtualizar(filho.get()) || filho->tipo() == tipo_caixa::popup || filho->tem(flag_estilo::altura_percentual))
            continue;

        // --- HORIZONTAL: altura justa é a maior altura entre os filhos (+ paddings) ---
        if (is_horizontal && it_caixa->tem(flag_estilo::altura_justa)) {
            float height_com_padding =
                filho->m_estilo.m_altura +
                filho->m_estilo.m_padding.y * 2 +
                it_caixa->m_estilo.m_padding_geral.y * 2;
            if (quebra_linha) height_com_padding += mais_alto;
            mais_alto = std::max(mais_alto, height_com_padding);
            it_caixa->m_estilo.m_altura = mais_alto;
        }
        // --- VERTICAL: soma todas as alturas (+ paddings) para ter "altura justa" ---
        else if (!is_horizontal && it_caixa->tem(flag_estilo::altura_justa)) {
            // Altura do filho + padding interno (top+bottom) + padding geral (top+bottom)
            float h = filho->m_estilo.m_altura
                    + filho->m_estilo.m_padding.y * 2
                    + it_caixa->m_estilo.m_padding_geral.y * 2;
            acumulado_altura += h;
        }
        quebra_linha = filho->tem(flag_estilo::quebrar_linha);
    }

    // Depois de somar todos os filhos, aplica à caixa pai (apenas no caso vertical)
    if (!is_horizontal && it_caixa->tem(flag_estilo::altura_justa)) {
        it_caixa->m_estilo.m_altura = acumulado_altura;
    }
}

// Atualizar Hierarquia De Traz pra Frente
void interface::atualizarHDTF(caixa* it_caixa, std::function<void(caixa*)> func) {
    if(!it_caixa) return;
    // Percorre os filhos de trás pra frente
    for (auto it = it_caixa->m_filhos.rbegin(); it != it_caixa->m_filhos.rend(); ++it) {
        // Chama recursivamente antes de atualizar a caixa atual
        atualizarHDTF(it->get(), func);
        
        func((*it).get());
    }
}
void interface::desenhar(caixa* c) {
    if(c) {
        c->m_projecao = projecao_viewport;
        c->desenhar(VAO);
        for(auto& filho : c->m_filhos) {
            if(     filho->m_estilo.m_limites.x < c->m_estilo.m_limites.x + c->m_estilo.m_limites.z
                    && filho->m_estilo.m_limites.y < c->m_estilo.m_limites.y + c->m_estilo.m_limites.w
                    && filho->m_estilo.m_ativo)
            desenhar(filho.get());
        }
    }
}

// \brief Inicia com a janela da engine
interface::interface() {
    m_raiz = std::make_unique<caixa>();
    m_window = motor::obter().m_janela.get();
}

// \brief Inicia com uma janela própria
interface::interface(ijanela* window) {
    m_raiz = std::make_unique<caixa>();
    m_window = window;
}

caixa* interface::obterRaiz() {
    return m_raiz.get();
}

void interface::inicializar() {
    sistema::inicializar();
    gerarBuffers();
}

interface::~interface() {
}

void interface::gerarBuffers()
{
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
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // configuração do VAO
        glBindVertexArray(VAO);

        // configuração do VBO (dados do quadrado)
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // configuração do EBO (indices)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // configuração do atributo de posição (layout location = 0)
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // configuração do atributo UV (layout location = 1)
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
}

bool interface::deveAtualizar(caixa* it_caixa) {
    if(!it_caixa) return false;
    if (!it_caixa->m_estilo.m_ativo) {
        return false; // Não atualiza caso a caixa esteja desativada 
    }

    if(it_caixa->m_estilo != it_caixa->m_estilo_antigo) {
        return true;
    }

    // Atualização forçada por evento de entrada
    if(m_window
            && (motor::obter().m_inputs->m_estado_mouse == GLFW_PRESS 
                || motor::obter().m_inputs->m_estado_tecla == GLFW_PRESS)) {
        return true;
    }
    it_caixa->m_estilo_antigo = it_caixa->m_estilo;
    return false;
}

void interface::configOpenglState() const {
    glCullFace(GL_FRONT);
    glDisable(GL_DEPTH_TEST);
    glViewport(0, 0,
        m_window->obterTamanho().x,
        m_window->obterTamanho().y
            );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);
}
 
/*
void interface::trazer(caixa* alvo) {
    if(alvo == nullptr) return;
    auto it = std::remove_if(m_floating.begin(), m_floating.end(),
        [alvo](const std::shared_ptr<caixa>& ptr) {
            return ptr.get() == alvo;
        });
    
    // guarda temporário
    auto ptr = *it;
    if (it != m_floating.end()) {
        m_floating.erase(it, m_floating.end());
        // adiciona no final (topo)
        m_floating.push_back(ptr);
    } else return;
}
*/

void interface::deconfigOpenglState() const {
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
}

void interface::renderizar() {
    configOpenglState();
    desenhar(m_raiz.get());
    for(size_t i = 0; i < m_floating.size(); i++) {
        auto filho = m_floating[i];
        if(filho->m_estilo.m_ativo)
        desenhar(filho.get());
    }
    for(size_t i = 0; i < m_popups.size(); i++) {
        auto filho = m_popups[i];
        if(filho->m_estilo.m_ativo)
        desenhar(filho.get());
    }
    deconfigOpenglState();
}
void interface::atualizar() {
    m_window->definirCursor(janela::cursor::seta);
    projecao_viewport = glm::ortho(0.f, static_cast<float>(m_window->obterTamanho().x),
        static_cast<float>(m_window->obterTamanho().y), 0.f);
    // atualiza dimensões da m_raiz
    if(!m_raiz) throw std::runtime_error("Raiz não definida!");
    m_raiz->m_estilo.m_limites = {0.f, 0.f,
        static_cast<float>(m_window->obterTamanho().x),
        static_cast<float>(m_window->obterTamanho().y)};

    chamarFuncoes(m_raiz.get());
    atualizarHDTF(m_raiz.get(), [this](auto* no) { 
            atualizarLJ(no);
            atualizarAJ(no);
            });
    atualizarFilhos(m_raiz.get());

    for(size_t i = 0; i < m_floating.size(); i++) {
        auto filho = m_floating[i];
        chamarFuncoes(filho.get());
        atualizarFilhos(filho.get());
    }
    for(size_t i = 0; i < m_popups.size(); i++) {
        auto filho = m_popups[i];
        chamarFuncoes(filho.get());
        atualizarFilhos(filho.get());
    }
}


void interface::organizarLinha(caixa* it_caixa,
                                bool is_horizontal,
                                const ivet2 range_filhos,
                                const fvet2& unidade_crescimento,
                                const fvet2& espaco_ocupado,
                                fvet2& cursor)
{
    float maior = 0.f; // o maior elemento
    // verificar se o pai tem a flag de alinhamento central
    if (it_caixa->tem(flag_estilo::alinhamento_central)) {
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
    } else if (it_caixa->tem(flag_estilo::alinhamento_fim)) {
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
        filho->m_estilo.m_limites.z = filho->tem(flag_estilo::largura_percentual) ? filho->m_estilo.m_largura * unidade_crescimento.x : filho->m_estilo.m_largura;
        filho->m_estilo.m_limites.w = filho->tem(flag_estilo::altura_percentual) ? filho->m_estilo.m_altura * unidade_crescimento.y : filho->m_estilo.m_altura;

        filho->m_estilo.m_limites.x = cursor.x + filho->m_estilo.m_padding.x + it_caixa->m_estilo.m_padding_geral.x;
        filho->m_estilo.m_limites.y = cursor.y + filho->m_estilo.m_padding.y + it_caixa->m_estilo.m_padding_geral.y;
        if (is_horizontal) {
            maior = std::max(maior, filho->m_estilo.m_limites.w + filho->m_estilo.m_padding.y * 2 + it_caixa->m_estilo.m_padding_geral.y * 2); // maior altura
            cursor.x += filho->m_estilo.m_limites.z + filho->m_estilo.m_padding.x + it_caixa->m_estilo.m_padding_geral.x;
        } else {
            maior = std::max(maior, filho->m_estilo.m_limites.z+ filho->m_estilo.m_padding.x * 2 + it_caixa->m_estilo.m_padding_geral.x * 2); // maior largura
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

void interface::processarDimensaoModular(caixa* filho, fvet2& crescimento_total, fvet2& espaco_ocupado) {
    if(filho->tem(flag_estilo::largura_percentual)) 
        if(filho->m_pai && filho->m_pai->m_estilo.m_orientacao_modular == estilo::orientacao::horizontal)
            crescimento_total.x += filho->m_estilo.m_largura;
        else
            crescimento_total.x = std::max(crescimento_total.x, filho->m_estilo.m_largura);
    
    if(filho->tem(flag_estilo::altura_percentual))
        if(filho->m_pai && filho->m_pai->m_estilo.m_orientacao_modular == estilo::orientacao::vertical)
            crescimento_total.y += filho->m_estilo.m_altura;
        else
            crescimento_total.y = std::max(crescimento_total.y, filho->m_estilo.m_altura);
        
    if(!filho->tem(flag_estilo::altura_percentual))
        if(filho->m_pai && filho->m_pai->m_estilo.m_orientacao_modular == estilo::orientacao::vertical)
            espaco_ocupado.y += filho->m_estilo.m_altura + filho->m_estilo.m_padding.y;
    if(!filho->tem(flag_estilo::largura_percentual))
        if(filho->m_pai && filho->m_pai->m_estilo.m_orientacao_modular == estilo::orientacao::horizontal)
            espaco_ocupado.x += filho->m_estilo.m_largura + filho->m_estilo.m_padding.x;
}

void interface::processarModular(caixa* it_caixa) {
    // se a caixa for modular, atualizar limites dos filhos
    if(it_caixa->tem(flag_estilo::modular)) {

    const bool is_horizontal = it_caixa->m_estilo.m_orientacao_modular == estilo::orientacao::horizontal;

    fvet2 espaco_disponivel = fvet2(it_caixa->m_estilo.m_limites.z, it_caixa->m_estilo.m_limites.w), // espaço disponível para redimensionamento de filhos
          espaco_ocupado = fvet2(0.f, 0.f),           // espaço ocupado pelos filhos
          cursor            = fvet2(it_caixa->m_estilo.m_limites.x, it_caixa->m_estilo.m_limites.y),
          crescimento_total = fvet2(0.f, 0.f);  // total ocupado pelo crescimento dos filhos.
   
    espaco_ocupado.x += it_caixa->m_estilo.m_padding_geral.x*2;
    espaco_ocupado.y += it_caixa->m_estilo.m_padding_geral.y*2;
    // calcula espaços e aplica dimenções
    ivet2 range_filhos = ivet2(0, 0);
    bool finalizar_linha;
    for(size_t i = 0; i < it_caixa->m_filhos.size(); i++) {
        auto& filho = it_caixa->m_filhos[i];
        finalizar_linha = (i == it_caixa->m_filhos.size() - 1) || (filho->tem(flag_estilo::quebrar_linha));
        
        if(filho->tipo() != tipo_caixa::popup) {
            if(is_horizontal)
                if(!finalizar_linha) espaco_ocupado.x += it_caixa->m_estilo.m_padding_geral.x;
            else
                if(!finalizar_linha) espaco_ocupado.y += it_caixa->m_estilo.m_padding_geral.y;
    
            processarDimensaoModular(filho.get(),
                    crescimento_total,
                    espaco_ocupado
                    );
            } 
        else {
                finalizar_linha = (i == it_caixa->m_filhos.size() - 1);
        }
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
                espaco_ocupado.x += it_caixa->m_estilo.m_padding_geral.x;
                crescimento_total.x = 0;
            }
            else {
                espaco_disponivel.y = it_caixa->m_estilo.m_limites.w;
                espaco_ocupado.y = 0;
                espaco_ocupado.y += it_caixa->m_estilo.m_padding_geral.y;
                crescimento_total.y = 0;
            }    
            // \}
        }
    }
    }
}

void interface::chamarFuncoes(caixa* it_caixa) {
    if(!it_caixa) return;
    else if (it_caixa->tipo() == tipo_caixa::botao) {
        auto btn = dynamic_cast<elementos::botao*>(it_caixa);
        if(btn->pressionado() && btn->m_use_funcao) btn->m_funcao();
    }
    for (auto& filho : it_caixa->m_filhos) {
        chamarFuncoes(filho.get());
    }
}
void interface::atualizarFilhos(caixa* it_caixa) {
    if (!it_caixa) throw std::runtime_error("Caixa nula sendo atualizada.");
    if (deveAtualizar(it_caixa)) processarModular(it_caixa);
    it_caixa->atualizar();

    // Atualiza recursivamente os filhos
    for (auto& filho : it_caixa->m_filhos) {
        if(!filho) {
            it_caixa->remover(filho.get());
            continue;
        }
        atualizarFilhos(filho.get());
    }
}
        
void interface::resetRoot() {
    if(m_raiz == nullptr)return;
    m_floating.clear();
    m_popups.clear();
    m_raiz.reset();
    m_raiz = std::make_unique<caixa>();
} 

void interface::remover(caixa* c) {
    //m_floating.erase(c->uid);
}
        
elementos::popup* interface::novo_popup(elementos::botao* ref, bool e) {
    auto nova_caixa = std::make_shared<elementos::popup>(ref, e);
    nova_caixa->configurar();
    auto* ptr = nova_caixa.get();
    m_popups.push_back(nova_caixa);
    return ptr;
}
