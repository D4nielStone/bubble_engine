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
 * @file area_de_texto.cpp
 */

#include "nucleo/engine.hpp"
#include "elementos/area_de_texto.hpp"
using namespace becommons;

bool elementos::area_de_texto::selecionado() {
    bool pressionado = motor::obter().m_inputs->obter(inputs::MOUSE_E);
    bool justPressed = pressionado && !m_mouse_antes_pressionado;

    // atualiza flag de cursor sobre a área
    m_mouse_cima = mouseEmCima();

    if (m_mouse_cima) {
        motor::obter().m_janela->definirCursor(janela::cursor::i);
    }
    if (justPressed) {
        // seleciona somente se o clique começar dentro da área
        m_selecionado = m_mouse_cima;
        m_pipe_offset = m_buffer.size();
    }

    // atualiza estado para o próximo frame
    m_mouse_antes_pressionado = pressionado;
    return m_selecionado;
}

            
void elementos::area_de_texto::atualizarBuffer() {
                m_pipe_offset = m_buffer.size();
                
                auto &input = motor::obter().m_inputs;
                if (motor::obter().m_inputs->obter(inputs::ENTER) || motor::obter().m_inputs->obter(inputs::KP_ENTER))
                    m_buffer += '\n'; // break line
                if(input->m_backspace_pressionado || input->m_backspace_repetido)
                    apagar();
                else if(input->m_letra_pressionada)
                    inserirLetra(input->m_ultima_letra);
                if(num_ptr) {
                    if(m_buffer.empty() || m_buffer == "." || m_buffer == "-") *num_ptr = 0;
                    else *num_ptr = std::stof(m_buffer);
                } else if (str_ptr) {
                    *str_ptr = m_buffer;
                }
                m_teclado_foi_solto = false;
            }
