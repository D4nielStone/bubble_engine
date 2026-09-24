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
 * @file text_box.hpp
 */

#pragma once
#include "commons_namespace.hpp"
#include "image.hpp"
#include "text.hpp"
#include "text_area.hpp"
#include <memory>

namespace COMMONS_NS {
    namespace elements {
        class text_box : public text_area {
            private:
                std::string m_etiqueta {""};
                color m_color_antiga, m_color_increment { 0.1f, 0.1f, 0.1f };
                float m_borda_antiga, m_increment_borda { 1.f };
                std::string m_display {""};
                elements::text* m_text_ptr;
            public:
                text_box() = default;
                text_box(const std::string& etiqueta, const std::string& buffer_ini = "") : m_etiqueta(etiqueta) {m_buffer = buffer_ini;};
                text_box(const std::string& etiqueta, std::string* buffer) : m_etiqueta(etiqueta), text_area(buffer) {};
                text_box(const std::string& etiqueta, float* f_ptr) : m_etiqueta(etiqueta), text_area(f_ptr) {};
                void configure() override {
                    m_style.m_general_padding = { 5, 2 };
                    m_style.m_style_flag |= style_flag::modular;
                    m_borda_antiga = m_style.m_espessura_borda;
                    m_color_antiga = m_style.m_color_borda.b;
                    m_text_ptr = add<text>(&m_display);
                    m_style.m_height = m_text_ptr->get_height(m_display) + m_style.m_general_padding.y * 2;
                    if(has(style_flag::center_alignment))
                        m_text_ptr->m_text_flags |= text_flags::center_alignment;
                    if(has(style_flag::end_alignment))
                        m_text_ptr->m_text_flags |= text_flags::end_alignment;
                };
                void update() {
                    // style
                    m_style.m_color_borda = selected() ? m_color_antiga + m_color_increment : m_color_antiga;
                    m_style.m_espessura_borda = m_selected ? m_borda_antiga + m_increment_borda : m_borda_antiga;
                    // buffer
                    if(m_selected) updateBuffer();
                    if(m_buffer.empty() && !m_etiqueta.empty()) {
                        m_display = m_etiqueta; m_text_ptr->m_style.m_color_fundo.a = 0.5;
                    }
                    else {
                        m_display = m_buffer; m_text_ptr->m_style.m_color_fundo.a = 1;
                    }
                    m_style.m_height = m_text_ptr->get_height(m_display) + m_style.m_general_padding.y * 2;

                    m_text_ptr->m_style.m_width = m_text_ptr->get_width(m_text_ptr->m_text_phrase);
                    m_text_ptr->m_style.m_height = m_text_ptr->get_height(m_text_ptr->m_text_phrase);
                }
        };
    }
}
