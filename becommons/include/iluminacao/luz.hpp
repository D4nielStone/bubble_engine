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
 * @file luz.hpp
 */

#pragma once
#include "util/cor.hpp"

namespace iluminacao {
    // \brief Estrutura de shadow mapping para luzes que projetam sombra
    struct mapa_sombra {
        GLuint m_fbo;
        GLuint m_tex_profundidade;
        unsigned int m_largura, m_altura;
    };
    enum class tipo {
        direcional, 
        pontual,
        holofote
    };
    // \brief Base para iluminação da engine
    struct luz {
        tipo m_tipo;
        cor m_cor { 1.f };
        float intensidade { 1.f };
        bool sombrear { false };
        bool ativada { false };

        mapa_sombra m_sombra;

        // \brief Construtores:
        luz(tipo t) : m_tipo(t) {/*...*/};
        virtual ~luz() = default;
    };
}
