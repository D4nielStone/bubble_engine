/** @copyright 
MIT LicenseCopyright (c) 2025 Daniel Oliveira

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
 * @file botao.hpp
 * @author Daniel O. dos Santos
 *
 * @licence MIT License
 */

#pragma once
#include <functional>
#include "namespace.hpp"
#include "util/caixa.hpp"
#include "inputs/inputs.hpp"
#include "os/janela.hpp"

namespace BECOMMONS_NS {
    namespace elementos {
        struct area_toque : caixa
        {
            /// Flags de ativação
            bool m_gatilho = false, m_pressionado, m_mouse_cima { false };
            std::function<void()> m_funcao;
            bool* m_interruptor{nullptr};
            /// Construtor padrão
            area_toque(const std::function<void()> &fctn) : m_funcao(fctn) {};
            area_toque(bool* ptr = nullptr) : m_interruptor(ptr) {};
            /// Atualiza função de ativação
            /// @returns Se está presionado ou não
            bool mouseEmCima() {
                /// vetor2 do mouse
                auto m = obterMouse();
                m_mouse_cima = (m.x > m_limites.x && m.x < m_limites.z + m_limites.x &&
                   m.y > m_limites.y && m.y < m_limites.w + m_limites.y);
                return m_mouse_cima;
            }
            virtual bool pressionado() {
                /// Reset do flag m_pressionado
                m_pressionado = false;
                /// Se não está pressionado, reseta gatilho
                if(!instanciaJanela->m_inputs.isKeyPressed("MouseE"))
                    m_gatilho = false;
                /// Caso dentro do campo
                if(mouseEmCima()) {
                    /// Define cursor para mão
                    instanciaJanela->defCursor(janela::cursor::mao);
                    /// Caso o gatilho esteja desativado e o mouse esquerdo tocado
                    if(!m_gatilho && instanciaJanela->m_inputs.isKeyPressed("MouseE")) {
                        /// Se m_interruptor é diferente de nullptr
                        if(m_interruptor) {
                            /// Lógica de inversão
                            if(*m_interruptor) *m_interruptor = false;
                            else               *m_interruptor = true;
                        } else /// Se interruptor é nulo, define m_pressionado como true
                            m_pressionado = true;
                    }
                }
                if(!m_gatilho && instanciaJanela->m_inputs.isKeyPressed("MouseE"))
                    m_gatilho = true;
                return m_pressionado;
            }
        };
    }
}
