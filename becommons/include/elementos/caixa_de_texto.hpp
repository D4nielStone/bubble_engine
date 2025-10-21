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
 * @file caixa_de_texto.hpp
 */

#pragma once
#include "imagem.hpp"
#include "texto.hpp"
#include "area_de_texto.hpp"
#include <memory>

namespace becommons {
    namespace elementos {
        class caixa_de_texto : public area_de_texto {
            private:
                std::string m_etiqueta {""};
                cor m_cor_antiga, m_acrescimo_cor { 0.1f, 0.1f, 0.1f };
                float m_borda_antiga, m_acrescimo_borda { 1.f };
                std::string m_display {""};
                elementos::texto* m_texto_ptr;
            public:
                caixa_de_texto() = default;
                caixa_de_texto(const std::string& etiqueta, const std::string& buffer_ini = "") : m_etiqueta(etiqueta) {m_buffer = buffer_ini;};
                caixa_de_texto(const std::string& etiqueta, std::string* buffer) : m_etiqueta(etiqueta), area_de_texto(buffer) {};
                caixa_de_texto(const std::string& etiqueta, float* f_ptr) : m_etiqueta(etiqueta), area_de_texto(f_ptr) {};
                void configurar() override {
                    m_filhos.clear();
                    m_estilo.m_corte = true;
                    m_estilo.m_padding_geral = { 5, 2 };
                    m_estilo.m_flag_estilo |= flag_estilo::modular;
                    m_borda_antiga = m_estilo.m_espessura_borda;
                    m_cor_antiga = m_estilo.m_cor_borda.b;
                    m_texto_ptr = adicionar<texto>(&m_display);
                    m_estilo.m_altura = m_texto_ptr->obterAltura(m_display) + m_estilo.m_padding_geral.y * 2;
                    if(tem(flag_estilo::alinhamento_central))
                        m_texto_ptr->m_texto_flags |= flags_texto::alinhamento_central;
                    if(tem(flag_estilo::alinhamento_fim))
                        m_texto_ptr->m_texto_flags |= flags_texto::alinhamento_fim;
                    if(!tem(flag_estilo::largura_percentual)) m_estilo.m_flag_estilo |= flag_estilo::largura_justa;
                };
                void atualizar() {
                    // estilo
                    m_estilo.m_cor_borda = selecionado() ? m_cor_antiga + m_acrescimo_cor : m_cor_antiga;
                    m_estilo.m_espessura_borda = m_selecionado ? m_borda_antiga + m_acrescimo_borda : m_borda_antiga;
                    // buffer
                    if(m_selecionado) atualizarBuffer();
                    if(m_buffer.empty() && !m_etiqueta.empty()) {
                        m_display = m_etiqueta; m_texto_ptr->m_estilo.m_cor_fundo.a = 0.5;
                    }
                    else {
                        m_display = m_buffer; m_texto_ptr->m_estilo.m_cor_fundo.a = 1;
                    }
                    m_estilo.m_altura = m_texto_ptr->obterAltura(m_display) + m_estilo.m_padding_geral.y * 2;

                    m_texto_ptr->m_estilo.m_largura = m_texto_ptr->obterLargura(m_texto_ptr->m_texto_frase);
                    m_texto_ptr->m_estilo.m_altura = m_texto_ptr->obterAltura(m_texto_ptr->m_texto_frase);
                }
        };
    } 
}
