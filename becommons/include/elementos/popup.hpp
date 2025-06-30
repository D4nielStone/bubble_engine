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
#include "becommons_namespace.hpp"
#include "area_de_toque.hpp"
#include <memory>
#include <cmath>

namespace BECOMMONS_NS {
    namespace elementos {
        class popup : public area_de_toque {
            private:
                botao* m_referencia {nullptr};
            public:
                popup() {
                }
                void configurar() override {
                    if (m_pai && m_pai->tipo() == tipo_caixa::botao)
                        m_referencia = static_cast<botao*>(m_pai);
                    m_estilo.m_cor_fundo = cor(0.1f);
                    m_estilo.m_cor_borda = cor(0.07f);
                    m_estilo.m_flag_estilo |= flag_estilo::largura_justa | flag_estilo::altura_justa;
                    m_estilo.m_orientacao_modular = estilo::orientacao::vertical;
                    m_estilo.m_padding_geral = {3, 3};
                }
                void atualizar() override {
                    if(!m_referencia) return;
                    m_estilo.m_ativo = m_estilo.m_ativo ? m_referencia->mouseEmCima() || mouseEmCima() : m_referencia->mouseEmCima();
                    float mais_largo = 20.f;
                    m_estilo.m_limites.w = m_estilo.m_padding_geral.y;
                    for(auto& filho : m_filhos) {
                        mais_largo = std::max(filho->m_estilo.m_limites.z+m_estilo.m_padding_geral.x*2, mais_largo);
                        m_estilo.m_limites.w += filho->m_estilo.m_limites.w + m_estilo.m_padding_geral.y;
                    }
                    if(m_estilo.m_ativo) {
                        m_estilo.m_limites.z = std::lerp<float>(m_estilo.m_limites.z, mais_largo, 0.5);
                        m_estilo.m_limites.x = std::lerp<float>(m_estilo.m_limites.x, m_referencia->m_estilo.m_limites.x- m_estilo.m_limites.z + 5, 0.1);
                    } else {
                        m_estilo.m_limites.z = 20;
                        m_estilo.m_limites.x = m_referencia->m_estilo.m_limites.x;
                    }
                    m_estilo.m_limites.y = m_referencia->m_estilo.m_limites.y;
                }
                tipo_caixa tipo() const override {
                    return tipo_caixa::popup;
                }
        };
    }
}
