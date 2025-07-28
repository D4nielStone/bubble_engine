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
 * @file area_de_toque.cpp
 */

#include "nucleo/engine.hpp"
#include "elementos/area_de_toque.hpp"
using namespace becommons;

bool elementos::area_de_toque::pressionado() {
                // Reset do flag m_pressionado
                m_pressionado = false;
                // Se não está pressionado, reseta gatilho
                if(!inputs::obter(inputs::MOUSE_E))
                    m_gatilho = false;

                // Caso dentro do campo
                if(mouseEmCima() && s_contagem_areas <= 1) {
                    // Define cursor para mão
                    motor::obter().m_janela->definirCursor(janela::cursor::mao);
                    // Caso o gatilho esteja desativado e o mouse esquerdo tocado
                    if(!m_gatilho && inputs::obter(inputs::MOUSE_E)) {
                        m_arrastando = true;
                        // Se m_interruptor é diferente de nullptr
                        if(m_interruptor) {
                            // Lógica de inversão
                            if(*m_interruptor) *m_interruptor = false;
                            else               *m_interruptor = true;
                        } else // Se interruptor é nulo, define m_pressionado como true
                            m_pressionado = true;
                    }
                }
                if(!m_gatilho && inputs::obter(inputs::MOUSE_E))
                    m_gatilho = true;
                return m_pressionado;
            }
