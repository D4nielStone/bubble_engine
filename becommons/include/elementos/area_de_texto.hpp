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
 * @file area_de_texto.hpp
 * @author Daniel O. dos Santos
 */

#pragma once
#include "becommons_namespace.hpp"
#include "util/caixa.hpp"
#include "inputs/inputs.hpp"
#include "os/janela.hpp"

namespace BECOMMONS_NS {
    namespace elementos {
        /** 
         * @struct area_de_texto
         * Abstração para elementos com espaço de texto
         * Exemplo: caixa_de_texto
         */ 
        class area_de_texto : public caixa {
        public:
            // indica se o elemento está selecionado
            bool m_selecionado { false };
            // indica se o cursor está sobre a área
            bool m_mouse_cima  { false };

        protected:
            // armazena o estado do botão do mouse no frame anterior para detectar clique único
            bool m_mouse_antes_pressionado { false }, m_teclado_foi_solto { true };
            int m_pipe_offset { 0 };
            std::string m_buffer {""};
        public:
            tipo_caixa tipo() const override { return tipo_caixa::caixa_de_texto; }

            /**
             * Retorna true se o elemento estiver selecionado.
             * A seleção só ocorre quando há transição de não pressionado para pressionado
             * dentro da área de toque, evitando cliques já iniciados fora.
             */
            bool selecionado() {
                bool pressionado = inputs::obter(inputs::MOUSE_E);
                bool justPressed = pressionado && !m_mouse_antes_pressionado;

                // atualiza flag de cursor sobre a área
                m_mouse_cima = mouseEmCima();

                if (justPressed) {
                    // seleciona somente se o clique começar dentro da área
                    m_selecionado = m_mouse_cima;
                }

                // atualiza estado para o próximo frame
                m_mouse_antes_pressionado = pressionado;
                return m_selecionado;
            }
            /**
             * Obtenção do buffer de texto
             */
            std::string obterBuffer() const {
                return m_buffer;
            }
            /**
             * Verifica se o cursor está dentro dos limites da caixa.
             */
            bool mouseEmCima() {
                dvet2 m = inputs::obterMousePos();
                bool dentro = (
                    m.x > m_estilo.m_limites.x && 
                    m.x < m_estilo.m_limites.x + m_estilo.m_limites.z &&
                    m.y > m_estilo.m_limites.y &&
                    m.y < m_estilo.m_limites.y + m_estilo.m_limites.w
                );
                m_mouse_cima = dentro;

                if (dentro) {
                    janela::obterInstancia().defCursor(janela::cursor::i);
                }
                return dentro;
            }
            void inserirLetra(char c) {
                if(c == '\0') return;
                if(m_pipe_offset >= m_buffer.size()) {
                    m_buffer.push_back(c);
                    m_pipe_offset = m_buffer.size();
                }
                else
                    m_buffer.insert(m_pipe_offset, c, 1);
            }
            void apagar() {
                if(m_pipe_offset <= m_buffer.size() && m_pipe_offset > 0)
                m_buffer.erase(m_pipe_offset-1, m_pipe_offset);
                m_pipe_offset --;
            }
            void atualizarBuffer() {
                auto &input = janela::obterInstancia().m_inputs;
                    if(input.m_backspace_pressionado || input.m_backspace_repetido)
                        apagar();
                    else
                if(input.m_letra_pressionada)
                        inserirLetra(input.m_ultima_letra);
                m_teclado_foi_solto = false;
            }
        };
    }
}
