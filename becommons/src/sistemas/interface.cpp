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

#include "becommons_namespace.hpp"
#include "sistemas/interface.hpp"
#include "os/janela.hpp"
#include "depuracao/debug.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <stdexcept>

using namespace BECOMMONS_NS;

void interface::atualizarLJ(caixa* it_caixa) {
    const bool is_horizontal =
        it_caixa->m_estilo.m_orientacao_modular ==
        estilo::orientacao::horizontal;

    if (!deveAtualizar(it_caixa))
        return;

    float mais_largo = 0.f;
    float acumulado_largura = 0.f;

    for (auto& filho : it_caixa->m_filhos) {
        if (!deveAtualizar(filho.get()))
            continue;

        if (filho->tem(flag_estilo::largura_percentual))
            continue;

        if (!is_horizontal &&
            it_caixa->tem(flag_estilo::largura_justa)) {

            float width_com_padding =
                filho->m_estilo.m_largura +
                filho->m_estilo.m_padding.x * 2.f;

            mais_largo =
                std::max(mais_largo, width_com_padding);
        }
        else if (is_horizontal &&
                 it_caixa->tem(flag_estilo::largura_justa)) {

            float width_com_padding =
                filho->m_estilo.m_largura +
                filho->m_estilo.m_padding.x * 2.f;

            acumulado_largura += width_com_padding;
        }
    }

    if (!is_horizontal &&
        it_caixa->tem(flag_estilo::largura_justa)) {

        it_caixa->m_estilo.m_largura =
            mais_largo +
            it_caixa->m_estilo.m_padding_geral.x * 2.f;
    }
    else if (is_horizontal &&
             it_caixa->tem(flag_estilo::largura_justa)) {

        it_caixa->m_estilo.m_largura =
            acumulado_largura +
            it_caixa->m_estilo.m_padding_geral.x * 2.f;
    }
}

void interface::atualizarAJ(caixa* it_caixa) {
    const bool is_horizontal =
        it_caixa->m_estilo.m_orientacao_modular ==
        estilo::orientacao::horizontal;

    if (!deveAtualizar(it_caixa))
        return;

    float mais_alto = 0.f;
    float acumulado_altura = 0.f;

    for (auto& filho : it_caixa->m_filhos) {
        if (!deveAtualizar(filho.get()))
            continue;

        if (filho->tem(flag_estilo::altura_percentual))
            continue;

        if (is_horizontal &&
            it_caixa->tem(flag_estilo::altura_justa)) {

            float height_com_padding =
                filho->m_estilo.m_altura +
                filho->m_estilo.m_padding.y * 2.f;

            mais_alto =
                std::max(mais_alto, height_com_padding);
        }
        else if (!is_horizontal &&
                 it_caixa->tem(flag_estilo::altura_justa)) {

            float height_com_padding =
                filho->m_estilo.m_altura +
                filho->m_estilo.m_padding.y * 2.f;

            acumulado_altura += height_com_padding;
        }
    }

    if (is_horizontal &&
        it_caixa->tem(flag_estilo::altura_justa)) {

        it_caixa->m_estilo.m_altura =
            mais_alto +
            it_caixa->m_estilo.m_padding_geral.y * 2.f;
    }
    else if (!is_horizontal &&
             it_caixa->tem(flag_estilo::altura_justa)) {

        it_caixa->m_estilo.m_altura =
            acumulado_altura +
            it_caixa->m_estilo.m_padding_geral.y * 2.f;
    }
}

void interface::atualizarHDTF(
    caixa* it_caixa,
    std::function<void(caixa*)> func
) {
    for (auto it = it_caixa->m_filhos.rbegin();
         it != it_caixa->m_filhos.rend();
         ++it) {

        atualizarHDTF(it->get(), func);

        func(it->get());
    }
}

void interface::desenhar(caixa* c) {
    c->m_projecao = projecao_viewport;
    c->desenhar(VAO);

    for (auto& filho : c->m_filhos) {
        if (
            filho->m_estilo.m_limites.x <
                c->m_estilo.m_limites.x +
                c->m_estilo.m_limites.z
            &&
            filho->m_estilo.m_limites.y <
                c->m_estilo.m_limites.y +
                c->m_estilo.m_limites.w
            &&
            filho->m_estilo.m_ativo
        ) {
            desenhar(filho.get());
        }
    }
}

interface::interface() {
    m_raiz = std::make_unique<caixa>();
}

void interface::inicializar() {
    gerarBuffers();
}

interface::~interface() {
}

void interface::gerarBuffers() {
    float vertices[] = {
        0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(float),
        (void*)(2 * sizeof(float))
    );

    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

bool interface::deveAtualizar(caixa* it_caixa) {
    if (!it_caixa->m_estilo.m_ativo)
        return false;

    if (it_caixa->m_estilo != it_caixa->m_estilo_antigo)
        return true;

    if (
        janela::temInstancia() &&
        (
            janela::obterInstancia().m_inputs.m_estado_mouse ==
                GLFW_PRESS ||
            janela::obterInstancia().m_inputs.m_estado_tecla ==
                GLFW_PRESS
        )
    ) {
        return true;
    }

    it_caixa->m_estilo_antigo =
        it_caixa->m_estilo;

    return false;
}

void interface::configOpenglState() const {
    glCullFace(GL_FRONT);
    glDisable(GL_DEPTH_TEST);

    glViewport(
        0,
        0,
        janela::obterInstancia().tamanho.x,
        janela::obterInstancia().tamanho.y
    );

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);
}

void interface::deconfigOpenglState() const {
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
}

void interface::atualizar() {
    configOpenglState();

    janela::obterInstancia().defCursor(
        janela::cursor::seta
    );

    projecao_viewport = glm::ortho(
        0.f,
        static_cast<float>(
            janela::obterTamanhoJanela().x
        ),
        static_cast<float>(
            janela::obterTamanhoJanela().y
        ),
        0.f
    );

    if (!m_raiz)
        throw std::runtime_error(
            "Raiz não definida!"
        );

    m_raiz->m_estilo.m_limites = {
        0.f,
        0.f,
        static_cast<float>(
            janela::obterTamanhoJanela().x
        ),
        static_cast<float>(
            janela::obterTamanhoJanela().y
        )
    };

    chamarFuncoes(m_raiz.get());

    atualizarHDTF(
        m_raiz.get(),
        atualizarLJ
    );

    atualizarHDTF(
        m_raiz.get(),
        atualizarAJ
    );

    atualizarFilhos(
        m_raiz.get()
    );

    desenhar(
        m_raiz.get()
    );

    deconfigOpenglState();
}

void interface::organizarLinha(
    caixa* it_caixa,
    bool is_horizontal,
    const ivet2 range_filhos,
    const fvet2& unidade_crescimento,
    const fvet2& espaco_ocupado,
    fvet2& cursor
) {
    float maior = 0.f;

    if (it_caixa->tem(flag_estilo::alinhamento_central)) {
        if (is_horizontal) {
            float larguraPai =
                it_caixa->m_estilo.m_limites.z;

            cursor.x +=
                (larguraPai - espaco_ocupado.x) / 2.f;
        }
        else {
            float alturaPai =
                it_caixa->m_estilo.m_limites.w;

            cursor.y +=
                (alturaPai - espaco_ocupado.y) / 2.f;
        }
    }
    else if (it_caixa->tem(flag_estilo::alinhamento_fim)) {
        if (is_horizontal) {
            float larguraPai =
                it_caixa->m_estilo.m_limites.z;

            cursor.x +=
                larguraPai - espaco_ocupado.x;
        }
        else {
            float alturaPai =
                it_caixa->m_estilo.m_limites.w;

            cursor.y +=
                alturaPai - espaco_ocupado.y;
        }
    }

    int i = range_filhos.x;

    while (i < range_filhos.y) {
        auto& filho =
            it_caixa->m_filhos[i];

        float largura =
            filho->m_estilo.m_largura;

        float altura =
            filho->m_estilo.m_altura;

        if (filho->tem(flag_estilo::largura_percentual)) {
            largura =
                filho->m_estilo.m_largura *
                unidade_crescimento.x;
        }

        if (filho->tem(flag_estilo::altura_percentual)) {
            altura =
                filho->m_estilo.m_altura *
                unidade_crescimento.y;
        }

        filho->m_estilo.m_limites.z =
            largura;

        filho->m_estilo.m_limites.w =
            altura;

        filho->m_estilo.m_limites.x =
            cursor.x +
            filho->m_estilo.m_padding.x +
            it_caixa->m_estilo.m_padding_geral.x;

        filho->m_estilo.m_limites.y =
            cursor.y +
            filho->m_estilo.m_padding.y +
            it_caixa->m_estilo.m_padding_geral.y;

        if (is_horizontal) {
            maior = std::max(
                maior,
                altura +
                filho->m_estilo.m_padding.y * 2.f
            );

            cursor.x +=
                largura +
                filho->m_estilo.m_padding.x * 2.f;
        }
        else {
            maior = std::max(
                maior,
                largura +
                filho->m_estilo.m_padding.x * 2.f
            );

            cursor.y +=
                altura +
                filho->m_estilo.m_padding.y * 2.f;
        }

        ++i;
    }

    if (is_horizontal) {
        cursor.y += maior;
        cursor.x =
            it_caixa->m_estilo.m_limites.x;
    }
    else {
        cursor.x += maior;
        cursor.y =
            it_caixa->m_estilo.m_limites.y;
    }
}

void interface::processarDimensaoModular(
    caixa* filho,
    fvet2& crescimento_total,
    fvet2& espaco_ocupado
) {
    if (
        filho->tem(flag_estilo::largura_percentual)
    ) {
        crescimento_total.x +=
            filho->m_estilo.m_largura;
    }
    else {
        espaco_ocupado.x +=
            filho->m_estilo.m_largura +
            filho->m_estilo.m_padding.x * 2.f;
    }

    if (
        filho->tem(flag_estilo::altura_percentual)
    ) {
        crescimento_total.y +=
            filho->m_estilo.m_altura;
    }
    else {
        espaco_ocupado.y +=
            filho->m_estilo.m_altura +
            filho->m_estilo.m_padding.y * 2.f;
    }
}

void interface::processarModular(caixa* it_caixa) {
    if (!it_caixa->tem(flag_estilo::modular))
        return;

    const bool is_horizontal =
        it_caixa->m_estilo.m_orientacao_modular ==
        estilo::orientacao::horizontal;

    const float largura_pai =
        it_caixa->m_estilo.m_limites.z;

    const float altura_pai =
        it_caixa->m_estilo.m_limites.w;

    float largura_padding =
        it_caixa->m_estilo.m_padding_geral.x * 2.f;

    float altura_padding =
        it_caixa->m_estilo.m_padding_geral.y * 2.f;

    float largura_disponivel =
        std::max(
            0.f,
            largura_pai - largura_padding
        );

    float altura_disponivel =
        std::max(
            0.f,
            altura_pai - altura_padding
        );

    fvet2 cursor = {
        it_caixa->m_estilo.m_limites.x,
        it_caixa->m_estilo.m_limites.y
    };

    size_t inicio_linha = 0;

    for (size_t i = 0;
         i < it_caixa->m_filhos.size();
         ++i) {

        auto& filho =
            it_caixa->m_filhos[i];

        if (!filho->m_estilo.m_ativo)
            continue;

        bool finalizar_linha =
            i ==
            it_caixa->m_filhos.size() - 1 ||
            filho->tem(flag_estilo::quebrar_linha);

        if (!finalizar_linha)
            continue;

        float largura_fixa = 0.f;
        float altura_fixa = 0.f;

        float percentual_largura = 0.f;
        float percentual_altura = 0.f;

        for (size_t j = inicio_linha;
             j <= i;
             ++j) {

            auto& atual =
                it_caixa->m_filhos[j];

            if (!atual->m_estilo.m_ativo)
                continue;

            if (
                atual->tem(
                    flag_estilo::largura_percentual
                )
            ) {
                percentual_largura +=
                    atual->m_estilo.m_largura;
            }
            else {
                largura_fixa +=
                    atual->m_estilo.m_largura +
                    atual->m_estilo.m_padding.x * 2.f;
            }

            if (
                atual->tem(
                    flag_estilo::altura_percentual
                )
            ) {
                percentual_altura +=
                    atual->m_estilo.m_altura;
            }
            else {
                altura_fixa +=
                    atual->m_estilo.m_altura +
                    atual->m_estilo.m_padding.y * 2.f;
            }
        }

        float largura_restante =
            std::max(
                0.f,
                largura_disponivel -
                largura_fixa
            );

        float altura_restante =
            std::max(
                0.f,
                altura_disponivel -
                altura_fixa
            );

        float unidade_largura =
            percentual_largura > 0.f
                ? largura_restante /
                  percentual_largura
                : 0.f;

        float unidade_altura =
            percentual_altura > 0.f
                ? altura_restante /
                  percentual_altura
                : 0.f;

        fvet2 unidade_crescimento = {
            unidade_largura,
            unidade_altura
        };

        float espaco_linha_x =
            largura_fixa;

        float espaco_linha_y =
            altura_fixa;

        for (size_t j = inicio_linha;
             j <= i;
             ++j) {

            auto& atual =
                it_caixa->m_filhos[j];

            if (!atual->m_estilo.m_ativo)
                continue;

            float largura =
                atual->m_estilo.m_largura;

            float altura =
                atual->m_estilo.m_altura;

            if (
                atual->tem(
                    flag_estilo::largura_percentual
                )
            ) {
                largura =
                    atual->m_estilo.m_largura *
                    unidade_largura;
            }

            if (
                atual->tem(
                    flag_estilo::altura_percentual
                )
            ) {
                altura =
                    atual->m_estilo.m_altura *
                    unidade_altura;
            }

            atual->m_estilo.m_limites.z =
                largura;

            atual->m_estilo.m_limites.w =
                altura;

            atual->m_estilo.m_limites.x =
                cursor.x +
                atual->m_estilo.m_padding.x +
                it_caixa->m_estilo.m_padding_geral.x;

            atual->m_estilo.m_limites.y =
                cursor.y +
                atual->m_estilo.m_padding.y +
                it_caixa->m_estilo.m_padding_geral.y;

            if (is_horizontal) {
                cursor.x +=
                    largura +
                    atual->m_estilo.m_padding.x * 2.f;
            }
            else {
                cursor.y +=
                    altura +
                    atual->m_estilo.m_padding.y * 2.f;
            }
        }

        if (is_horizontal) {
            cursor.y +=
                altura_fixa;

            cursor.x =
                it_caixa->m_estilo.m_limites.x;
        }
        else {
            cursor.x +=
                largura_fixa;

            cursor.y =
                it_caixa->m_estilo.m_limites.y;
        }

        inicio_linha = i + 1;
    }
}

void interface::chamarFuncoes(caixa* c) {
    for (auto& it_caixa : c->m_filhos) {
        switch (it_caixa->tipo()) {
            case tipo_caixa::caixa_de_texto: {
                auto ct =
                    static_cast<
                        elementos::caixa_de_texto*
                    >(it_caixa.get());

                ct->atualizar();

                break;
            }

            case tipo_caixa::botao: {
                auto btn =
                    static_cast<
                        elementos::botao*
                    >(it_caixa.get());

                if (
                    btn->pressionado() &&
                    btn->m_use_funcao
                ) {
                    btn->m_funcao();
                }

                break;
            }

            default:
                break;
        }

        chamarFuncoes(
            it_caixa.get()
        );
    }
}

void interface::atualizarFilhos(caixa* it_caixa) {
    if (!it_caixa)
        throw std::runtime_error(
            "Caixa nula sendo atualizada."
        );

    if (deveAtualizar(it_caixa))
        processarModular(it_caixa);

    for (auto& filho : it_caixa->m_filhos) {
        atualizarFilhos(
            filho.get()
        );
    }
}