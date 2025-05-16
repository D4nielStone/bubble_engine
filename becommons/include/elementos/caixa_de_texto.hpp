/** @copyright 
MIT License
Copyright (c) 2025 Daniel Oliveira

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
*/
/**
 * @file caixa_de_texto.hpp
 */

#pragma once
#include "becommons_namespace.hpp"
#include "imagem.hpp"
#include "texto.hpp"
#include "area_de_texto.hpp"
#include <memory>

namespace BECOMMONS_NS {
    namespace elementos {
        class caixa_de_texto : public area_de_texto {
            private:
                std::string m_buffer {""};
                cor m_cor_antiga, m_acrescimo_cor { 0.1f, 0.1f, 0.25f };
                float m_borda_antiga, m_acrescimo_borda { 1.f };
                bool* m_dica_bool;
            public:
                caixa_de_texto() = default;
                void configurar() override {
                    m_estilo.m_flag_estilo |= flag_estilo::modular;
                    m_estilo.m_padding_geral = { 5, 2 };
                    m_borda_antiga = m_estilo.m_espessura_borda;
                    m_cor_antiga = m_estilo.m_cor_borda.b;
                    auto m_texto = adicionarFilho<elementos::texto>(m_id + "_dica", "Digite um nome ...", cor(1, 1, 1, 0.5));
                    m_dica_bool = &m_texto->m_estilo.m_ativo;
                };
                std::string& obterBuffer() {
                    return m_buffer;
                }
                void atualizarInputs() {
                    m_estilo.m_cor_borda = selecionado() ? m_cor_antiga + m_acrescimo_cor : m_cor_antiga;
                    m_estilo.m_espessura_borda = m_selecionado ? m_borda_antiga + m_acrescimo_borda : m_borda_antiga;
                    *m_dica_bool = m_buffer.empty();
                }
        };
    } 
}
