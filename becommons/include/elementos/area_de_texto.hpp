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
 * @file area_de_texto.hpp
 * @author Daniel O. dos Santos
 */

#pragma once
#include "util/caixa.hpp"
#include "inputs/inputs.hpp"
#include "os/janela.hpp"
#include <cmath>

namespace becommons {
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
        protected:
            // armazena o estado do botão do mouse no frame anterior para detectar clique único
            bool m_mouse_antes_pressionado { false }, m_teclado_foi_solto { true };
            int m_pipe_offset { 0 };
            std::string m_buffer {""}, *str_ptr {nullptr};
            float *num_ptr { nullptr };
        public:
            bool m_break = false;
            area_de_texto() = default;
            area_de_texto(std::string* buffer) : str_ptr(buffer), m_buffer() {
                if(buffer) m_buffer = *buffer;
            }
            area_de_texto(float* f_ptr) : num_ptr(f_ptr), m_buffer() {
                if(num_ptr) {
                    m_buffer = std::to_string(*num_ptr);
                    if(m_buffer.find('.') != std::string::npos) {
                        // Remove trailing zeroes
                        m_buffer = m_buffer.substr(0, m_buffer.find_last_not_of('0')+1);
                        // If the decimal point is now the last character, remove that as well
                        if(m_buffer.find('.') == m_buffer.size()-1) {
                            m_buffer = m_buffer.substr(0, m_buffer.size()-1);
                        }
                    }
                }
            }
            tipo_caixa tipo() const override { return tipo_caixa::caixa_de_texto; }

            /**
             * Retorna true se o elemento estiver selecionado.
             * A seleção só ocorre quando há transição de não pressionado para pressionado
             * dentro da área de toque, evitando cliques já iniciados fora.
             */
            bool selecionado();
            /**
             * Obtenção do buffer de texto
             */
            std::string obterBuffer() const {
                return m_buffer;
            }
            void inserirLetra(char c) {
                if (c == '\0') return;
            
                if (num_ptr) {
                    // Aceita apenas números, ponto flutuante e sinal negativo no início
                    bool valido = 
                         (c >= '0' && c <= '9') ||
                        (c == '.' && m_buffer.find('.') == std::string::npos) || 
                        (c == '-' && m_pipe_offset == 0 && m_buffer.find('-') == std::string::npos); // apenas um sinal negativo no início
            
                    if (!valido) return;
                }
                m_buffer.push_back(c);
                m_pipe_offset = m_buffer.size();
            }
            void apagar() {
                if(!m_buffer.empty())
                m_buffer.pop_back();
                m_pipe_offset = m_buffer.size();
            }
            void atualizarBuffer();      
        };
    }
}
