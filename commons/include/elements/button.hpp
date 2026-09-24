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
 * @file button.hpp
 */

#pragma once
#include "commons_namespace.hpp"
#include "image.hpp"
#include "text.hpp"
#include "touch_area.hpp"
#include <memory>

namespace COMMONS_NS{
    namespace elements{
        class button : public touch_area {
        private:
            float old_fundo_alpha;
        public:
            box_type type() const override { return box_type::button; }
            /// @name Constructores com ponteiro para aparência
            /// @{
            button(const std::function<void()> &fctn, std::unique_ptr<text> m_text)    : touch_area(fctn) {
                m_children.push_back(std::move(m_text));
            }
            button(const std::function<void()> &fctn, std::unique_ptr<image> m_image)   : touch_area(fctn) {
                m_children.push_back(std::move(m_image));
            }
            button(bool* ptr, std::unique_ptr<image> m_image)                           : touch_area(ptr) {
                m_children.push_back(std::move(m_image));
            }
            button(bool* ptr, std::unique_ptr<text> m_text)                           : touch_area(ptr) {
                m_children.push_back(std::move(m_text));
            }
            /// @}
            /// @name Constructores de text simplificado
            /// @{
            button(const std::function<void()> &fctn, const std::string& txt) : touch_area(fctn) {
                auto m_text = std::make_unique<text>(txt);
                m_children.push_back(std::move(m_text));
            }
            button(bool* ptr, const std::string& txt) : touch_area(ptr) {
                auto m_text = std::make_unique<text>(txt);
                m_children.push_back(std::move(m_text));
            }
            button(bool* ptr, const std::string& txt, const std::string img, const unsigned int size = 20) : touch_area(ptr) {
                auto m_text = std::make_unique<text>(txt);
                auto m_image = std::make_unique<image>(img);
                m_image->m_style.m_height = size;
                m_image->m_style.m_width = size;
                m_children.push_back(std::move(m_image));
                m_children.push_back(std::move(m_text));
            }
            button(const std::function<void()> &fctn, const std::string& txt, const std::string img, const unsigned int size = 20) : touch_area(fctn) {
                auto m_text = std::make_unique<text>(txt);
                auto m_image = std::make_unique<image>(img);
                m_image->m_style.m_height = size;
                m_image->m_style.m_width = size;
                m_children.push_back(std::move(m_image));
                m_children.push_back(std::move(m_text));
            }
            /// @}
            ~button() {
            }
            bool pressionado() override {
                m_pressionado = touch_area::pressionado();
                m_style.m_color_borda.a = m_mouse_up ? 1.f : 0.f;
                return m_pressionado;
            }
            void configure() override {
                m_style.m_style_flag |= has(style_flag::largura_percentual) ? style_flag::modular : style_flag::largura_justa | style_flag::altura_justa | style_flag::modular;
                old_fundo_alpha = m_style.m_color_fundo.a;
            }
        };
    } // elements
} // namespace commons
