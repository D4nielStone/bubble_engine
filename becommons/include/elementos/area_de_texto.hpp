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
#include "namespace.hpp"
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
        struct area_de_texto : caixa {
            bool mouseEmCima() const {
                /// vetor2 do mouse
                dvet2 m = obterMouse();
                bool m_mouse_cima = (m.x > m_estilo.m_limites.x && m.x < m_estilo.m_limites.z + m_estilo.m_limites.x &&
                   m.y > m_estilo.m_limites.y && m.y < m_estilo.m_limites.w + m_estilo.m_limites.y);
                if(m_mouse_cima) {
                    janela::obterInstancia().defCursor(janela::cursor::i);
                    return true;
                }
            }
        };
    }
}
