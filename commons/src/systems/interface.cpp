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

#include "commons_namespace.hpp"
#include "systems/interface.hpp"
#include "os/window.hpp"
#include "debugging/debug.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <stdexcept>

using namespace COMMONS_NS;

void interface::update_width(box* it_caixa) {
    const bool is_horizontal =
        it_caixa->m_style.m_orientation_modular ==
        style::orientation::horizontal;

    if (!needs_update(it_caixa))
        return;

    float mais_largo = 0.f;
    float acumulado_largura = 0.f;

    for (auto& filho : it_caixa->m_children) {
        if (!needs_update(filho.get()))
            continue;

        if (filho->has(style_flag::largura_percentual))
            continue;

        if (!is_horizontal &&
            it_caixa->has(style_flag::largura_justa)) {

            float width_com_padding =
                filho->m_style.m_width +
                filho->m_style.m_padding.x * 2.f;

            mais_largo =
                std::max(mais_largo, width_com_padding);
        }
        else if (is_horizontal &&
                 it_caixa->has(style_flag::largura_justa)) {

            float width_com_padding =
                filho->m_style.m_width +
                filho->m_style.m_padding.x * 2.f;

            acumulado_largura += width_com_padding;
        }
    }

    if (!is_horizontal &&
        it_caixa->has(style_flag::largura_justa)) {

        it_caixa->m_style.m_width =
            mais_largo +
            it_caixa->m_style.m_general_padding.x * 2.f;
    }
    else if (is_horizontal &&
             it_caixa->has(style_flag::largura_justa)) {

        it_caixa->m_style.m_width =
            acumulado_largura +
            it_caixa->m_style.m_general_padding.x * 2.f;
    }
}

void interface::update_height(box* it_caixa) {
    const bool is_horizontal =
        it_caixa->m_style.m_orientation_modular ==
        style::orientation::horizontal;

    if (!needs_update(it_caixa))
        return;

    float mais_alto = 0.f;
    float acumulado_altura = 0.f;

    for (auto& filho : it_caixa->m_children) {
        if (!needs_update(filho.get()))
            continue;

        if (filho->has(style_flag::altura_percentual))
            continue;

        if (is_horizontal &&
            it_caixa->has(style_flag::altura_justa)) {

            float height_com_padding =
                filho->m_style.m_height +
                filho->m_style.m_padding.y * 2.f;

            mais_alto =
                std::max(mais_alto, height_com_padding);
        }
        else if (!is_horizontal &&
                 it_caixa->has(style_flag::altura_justa)) {

            float height_com_padding =
                filho->m_style.m_height +
                filho->m_style.m_padding.y * 2.f;

            acumulado_altura += height_com_padding;
        }
    }

    if (is_horizontal &&
        it_caixa->has(style_flag::altura_justa)) {

        it_caixa->m_style.m_height =
            mais_alto +
            it_caixa->m_style.m_general_padding.y * 2.f;
    }
    else if (!is_horizontal &&
             it_caixa->has(style_flag::altura_justa)) {

        it_caixa->m_style.m_height =
            acumulado_altura +
            it_caixa->m_style.m_general_padding.y * 2.f;
    }
}

void interface::updateHDTF(
    box* it_caixa,
    std::function<void(box*)> func
) {
    for (auto it = it_caixa->m_children.rbegin();
         it != it_caixa->m_children.rend();
         ++it) {

        updateHDTF(it->get(), func);

        func(it->get());
    }
}

void interface::draw(box* c) {
    c->m_projecao = projecao_viewport;
    c->draw(VAO);

    for (auto& filho : c->m_children) {
        if (
            filho->m_style.m_limits.x <
                c->m_style.m_limits.x +
                c->m_style.m_limits.z
            &&
            filho->m_style.m_limits.y <
                c->m_style.m_limits.y +
                c->m_style.m_limits.w
            &&
            filho->m_style.m_active
        ) {
            draw(filho.get());
        }
    }
}

interface::interface() {
    m_raiz = std::make_unique<box>();
}

void interface::setup(const std::shared_ptr<ecs>&) {
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

bool interface::needs_update(box* it_caixa) {
    if (!it_caixa->m_style.m_active)
        return false;

    if (it_caixa->m_style != it_caixa->m_style_antigo)
        return true;

    if (
        window::hasInstance() &&
        (
            window::get_instance().m_inputs.m_estado_mouse ==
                GLFW_PRESS ||
            window::get_instance().m_inputs.m_estado_tecla ==
                GLFW_PRESS
        )
    ) {
        return true;
    }

    it_caixa->m_style_antigo =
        it_caixa->m_style;

    return false;
}

void interface::configOpenglState() const {
    glCullFace(GL_FRONT);
    glDisable(GL_DEPTH_TEST);

    glViewport(
        0,
        0,
        window::get_instance().size.x,
        window::get_instance().size.y
    );

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);
}

void interface::deconfigOpenglState() const {
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
}

void interface::update(const std::shared_ptr<ecs>&) {
    configOpenglState();

    window::get_instance().defCursor(
        window::cursor::seta
    );

    projecao_viewport = glm::ortho(
        0.f,
        static_cast<float>(
            window::get_window_size().x
        ),
        static_cast<float>(
            window::get_window_size().y
        ),
        0.f
    );

    if (!m_raiz)
        throw std::runtime_error(
            "Raiz não definida!"
        );

    m_raiz->m_style.m_limits = {
        0.f,
        0.f,
        static_cast<float>(
            window::get_window_size().x
        ),
        static_cast<float>(
            window::get_window_size().y
        )
    };

    chamarFuncoes(m_raiz.get());

    updateHDTF(
        m_raiz.get(),
        update_width
    );

    updateHDTF(
        m_raiz.get(),
        update_height
    );

    update_children(
        m_raiz.get()
    );

    draw(
        m_raiz.get()
    );

    deconfigOpenglState();
}

void interface::organizarLinha(
    box* it_caixa,
    bool is_horizontal,
    const ivec2 range_filhos,
    const fvector_type2& unidade_crescimento,
    const fvector_type2& espaco_ocupado,
    fvector_type2& cursor
) {
    float maior = 0.f;

    if (it_caixa->has(style_flag::center_alignment)) {
        if (is_horizontal) {
            float larguraPai =
                it_caixa->m_style.m_limits.z;

            cursor.x +=
                (larguraPai - espaco_ocupado.x) / 2.f;
        }
        else {
            float alturaPai =
                it_caixa->m_style.m_limits.w;

            cursor.y +=
                (alturaPai - espaco_ocupado.y) / 2.f;
        }
    }
    else if (it_caixa->has(style_flag::end_alignment)) {
        if (is_horizontal) {
            float larguraPai =
                it_caixa->m_style.m_limits.z;

            cursor.x +=
                larguraPai - espaco_ocupado.x;
        }
        else {
            float alturaPai =
                it_caixa->m_style.m_limits.w;

            cursor.y +=
                alturaPai - espaco_ocupado.y;
        }
    }

    int i = range_filhos.x;

    while (i < range_filhos.y) {
        auto& filho =
            it_caixa->m_children[i];

        float width =
            filho->m_style.m_width;

        float height =
            filho->m_style.m_height;

        if (filho->has(style_flag::largura_percentual)) {
            width =
                filho->m_style.m_width *
                unidade_crescimento.x;
        }

        if (filho->has(style_flag::altura_percentual)) {
            height =
                filho->m_style.m_height *
                unidade_crescimento.y;
        }

        filho->m_style.m_limits.z =
            width;

        filho->m_style.m_limits.w =
            height;

        filho->m_style.m_limits.x =
            cursor.x +
            filho->m_style.m_padding.x +
            it_caixa->m_style.m_general_padding.x;

        filho->m_style.m_limits.y =
            cursor.y +
            filho->m_style.m_padding.y +
            it_caixa->m_style.m_general_padding.y;

        if (is_horizontal) {
            maior = std::max(
                maior,
                height +
                filho->m_style.m_padding.y * 2.f
            );

            cursor.x +=
                width +
                filho->m_style.m_padding.x * 2.f;
        }
        else {
            maior = std::max(
                maior,
                width +
                filho->m_style.m_padding.x * 2.f
            );

            cursor.y +=
                height +
                filho->m_style.m_padding.y * 2.f;
        }

        ++i;
    }

    if (is_horizontal) {
        cursor.y += maior;
        cursor.x =
            it_caixa->m_style.m_limits.x;
    }
    else {
        cursor.x += maior;
        cursor.y =
            it_caixa->m_style.m_limits.y;
    }
}

void interface::processarDimensaoModular(
    box* filho,
    fvector_type2& crescimento_total,
    fvector_type2& espaco_ocupado
) {
    if (
        filho->has(style_flag::largura_percentual)
    ) {
        crescimento_total.x +=
            filho->m_style.m_width;
    }
    else {
        espaco_ocupado.x +=
            filho->m_style.m_width +
            filho->m_style.m_padding.x * 2.f;
    }

    if (
        filho->has(style_flag::altura_percentual)
    ) {
        crescimento_total.y +=
            filho->m_style.m_height;
    }
    else {
        espaco_ocupado.y +=
            filho->m_style.m_height +
            filho->m_style.m_padding.y * 2.f;
    }
}

void interface::process_modular(box* it_caixa) {
    if (!it_caixa->has(style_flag::modular))
        return;

    const bool is_horizontal =
        it_caixa->m_style.m_orientation_modular ==
        style::orientation::horizontal;

    const float largura_pai =
        it_caixa->m_style.m_limits.z;

    const float altura_pai =
        it_caixa->m_style.m_limits.w;

    float largura_padding =
        it_caixa->m_style.m_general_padding.x * 2.f;

    float altura_padding =
        it_caixa->m_style.m_general_padding.y * 2.f;

    float largura_dispolevel =
        std::max(
            0.f,
            largura_pai - largura_padding
        );

    float altura_dispolevel =
        std::max(
            0.f,
            altura_pai - altura_padding
        );

    fvector_type2 cursor = {
        it_caixa->m_style.m_limits.x,
        it_caixa->m_style.m_limits.y
    };

    size_t inicio_linha = 0;

    for (size_t i = 0;
         i < it_caixa->m_children.size();
         ++i) {

        auto& filho =
            it_caixa->m_children[i];

        if (!filho->m_style.m_active)
            continue;

        bool finalizar_linha =
            i ==
            it_caixa->m_children.size() - 1 ||
            filho->has(style_flag::quebrar_linha);

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
                it_caixa->m_children[j];

            if (!atual->m_style.m_active)
                continue;

            if (
                atual->has(
                    style_flag::largura_percentual
                )
            ) {
                percentual_largura +=
                    atual->m_style.m_width;
            }
            else {
                largura_fixa +=
                    atual->m_style.m_width +
                    atual->m_style.m_padding.x * 2.f;
            }

            if (
                atual->has(
                    style_flag::altura_percentual
                )
            ) {
                percentual_altura +=
                    atual->m_style.m_height;
            }
            else {
                altura_fixa +=
                    atual->m_style.m_height +
                    atual->m_style.m_padding.y * 2.f;
            }
        }

        float largura_restante =
            std::max(
                0.f,
                largura_dispolevel -
                largura_fixa
            );

        float altura_restante =
            std::max(
                0.f,
                altura_dispolevel -
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

        fvector_type2 unidade_crescimento = {
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
                it_caixa->m_children[j];

            if (!atual->m_style.m_active)
                continue;

            float width =
                atual->m_style.m_width;

            float height =
                atual->m_style.m_height;

            if (
                atual->has(
                    style_flag::largura_percentual
                )
            ) {
                width =
                    atual->m_style.m_width *
                    unidade_largura;
            }

            if (
                atual->has(
                    style_flag::altura_percentual
                )
            ) {
                height =
                    atual->m_style.m_height *
                    unidade_altura;
            }

            atual->m_style.m_limits.z =
                width;

            atual->m_style.m_limits.w =
                height;

            atual->m_style.m_limits.x =
                cursor.x +
                atual->m_style.m_padding.x +
                it_caixa->m_style.m_general_padding.x;

            atual->m_style.m_limits.y =
                cursor.y +
                atual->m_style.m_padding.y +
                it_caixa->m_style.m_general_padding.y;

            if (is_horizontal) {
                cursor.x +=
                    width +
                    atual->m_style.m_padding.x * 2.f;
            }
            else {
                cursor.y +=
                    height +
                    atual->m_style.m_padding.y * 2.f;
            }
        }

        if (is_horizontal) {
            cursor.y +=
                altura_fixa;

            cursor.x =
                it_caixa->m_style.m_limits.x;
        }
        else {
            cursor.x +=
                largura_fixa;

            cursor.y =
                it_caixa->m_style.m_limits.y;
        }

        inicio_linha = i + 1;
    }
}

void interface::chamarFuncoes(box* c) {
    for (auto& it_caixa : c->m_children) {
        switch (it_caixa->type()) {
            case box_type::text_box: {
                auto ct =
                    static_cast<
                        elements::text_box*
                    >(it_caixa.get());

                ct->update();

                break;
            }

            case box_type::button: {
                auto btn =
                    static_cast<
                        elements::button*
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

void interface::update_children(box* it_caixa) {
    if (!it_caixa)
        throw std::runtime_error(
            "Caixa nula sendo atualizada."
        );

    if (needs_update(it_caixa))
        process_modular(it_caixa);

    for (auto& filho : it_caixa->m_children) {
        update_children(
            filho.get()
        );
    }
}
