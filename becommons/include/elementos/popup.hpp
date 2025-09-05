/** \copyright 
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
 * @file popup.hpp
 */

#pragma once
#include "area_de_toque.hpp"
#include <memory>
#include <cmath>

namespace becommons {
    namespace elementos {
        class popup : public area_de_toque {
            private:
                botao* m_referencia {nullptr};
                bool prevMouseE = false;
                int x_antigo = 0;
                int y_antigo = 0;
                float m_velocidade_lerp = 13.f;
                bool m_esquerdo = true;
            public:

                popup(botao* ref, bool e = true) : m_referencia(ref), m_esquerdo(e) {
                    m_estilo.m_orientacao_modular = estilo::orientacao::vertical;
                    m_estilo.m_ativo = false;
                }
                void configurar() override {
                    m_estilo.m_cor_fundo = cor(0.1f);
                    m_estilo.m_cor_borda = cor(0.07f);
                    m_estilo.m_flag_estilo |= flag_estilo::largura_justa | flag_estilo::altura_justa;
                    m_estilo.m_padding_geral = {3, 3};
                    m_estilo.m_ativo = false;
                }
                void atualizar() override;
                tipo_caixa tipo() const override {
                    return tipo_caixa::popup;
                }
        };
    }
}
