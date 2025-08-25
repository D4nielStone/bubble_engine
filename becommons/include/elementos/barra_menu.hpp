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
 * @file barra_menu.hpp
 */

#pragma once
#include "becommons/becommons.hpp"

// Estilos de caixa pré-definidos
using namespace becommons;
static estilo e, e2;

namespace custom {
    class barra_menu : public caixa {
        private:
            caixa* p_barra_menu;
        public:
            barra_menu() = default;

            void configurar() override {
                m_filhos.clear();
                m_estilo.m_flag_estilo |= flag_estilo::altura_justa | flag_estilo::largura_percentual;
                m_estilo.m_largura = 1;

                // Configuração da barra de menu
                p_barra_menu = adicionar<caixa>();
                p_barra_menu->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual;
                p_barra_menu->m_estilo.m_largura = 1;
                p_barra_menu->m_estilo.m_altura = 18;
                p_barra_menu->m_estilo.m_padding_geral = {1, 0};
                p_barra_menu->m_estilo.m_cor_fundo = cor(0.135f);
                p_barra_menu->m_estilo.m_cor_borda = cor(0.4f);

                // Configuração da barra de versão
                auto* barra_versao = adicionar<caixa>();
                barra_versao->m_estilo.m_flag_estilo |= flag_estilo::altura_justa | flag_estilo::largura_justa;
                barra_versao->adicionar<elementos::texto>(BUBBLE_VERSAO_COMPLETA_STR); // Exibe a string de versão completa
                barra_versao->m_estilo.m_cor_fundo = cor(0.11f);
            }
            void adicionar_botao(const std::string& nome, const std::function<void()>& func) {
                // Cria o botão
                auto* btn = p_barra_menu->adicionar<elementos::botao>(func, nome);
                btn->m_estilo.m_cor_fundo.a = 0.f;
                e = btn->m_estilo; // guarda como estilo de botoes
            }
            void adicionar_botao(const std::string& nome, const std::string& icone, const std::function<void()>& func) {
                // Cria o botão
                auto* btn = p_barra_menu->adicionar<elementos::botao>(func, nome, icone, 18);
                btn->m_estilo.m_cor_fundo.a = 0.f;
                e = btn->m_estilo; // guarda como estilo de botoes
            }
    };
}
