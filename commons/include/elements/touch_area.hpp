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
 * @file touch_area.hpp
 * @author Daniel O. dos Santos
 *
 *
 */

#pragma once
#include <functional>
#include "commons_namespace.hpp"
#include "util/box.hpp"
#include "inputs/inputs.hpp"
#include "os/window.hpp"

namespace COMMONS_NS {
    namespace elements {
        // @struct touch_area
        // Abstração para elements sensíveis ao toque.
        // Exemplo: button
        class touch_area : public box {
            public:
            // Flags de ativação
            bool m_gatilho = false, m_pressionado, m_mouse_up { false }, m_arrastando, m_use_funcao{ false };
            std::function<void()> m_funcao;
            bool* m_interruptor{nullptr};
            // Constructor default
            touch_area(const std::function<void()> &fctn) : m_funcao(fctn), m_use_funcao(true) {};
            touch_area(bool* ptr = nullptr) : m_interruptor(ptr), m_use_funcao(false) {};
            // Atualiza função de ativação
            // @returns Se está presionado ou não
            bool mouseEmCima() {
                // vector2 do mouse
                auto m = inputs::get_mouse_position();
                m_mouse_up = (m.x > m_style.m_limits.x && m.x < m_style.m_limits.z + m_style.m_limits.x &&
                   m.y > m_style.m_limits.y && m.y < m_style.m_limits.w + m_style.m_limits.y);
                return m_mouse_up;
            }
            virtual bool pressionado() {
                // Reset do flag m_pressionado
                m_pressionado = false;
                // Se não está pressionado, reseta gatilho
                if(!inputs::get(inputs::MOUSE_E))
                    m_gatilho = false;
                // Caso dentro do campo
                if(mouseEmCima()) {
                    // Define cursor para mão
                    window::get_instance().defCursor(window::cursor::mao);
                    // Caso o gatilho esteja desativado e o mouse esquerdo tocado
                    if(!m_gatilho && inputs::get(inputs::MOUSE_E)) {
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
                if(!m_gatilho && inputs::get(inputs::MOUSE_E))
                    m_gatilho = true;
                return m_pressionado;
            }
            bool arrastando() {
                if(!inputs::get(inputs::MOUSE_E))
                    m_arrastando = false;
                // Caso dentro do campo
                if(!m_arrastando && mouseEmCima() && inputs::get(inputs::MOUSE_E)) {
                    m_arrastando = true;
                }
                return m_arrastando;
            }
        };
    }
}
