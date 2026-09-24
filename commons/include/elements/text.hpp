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
 * @file text.hpp
 */

#pragma once
#include "commons_namespace.hpp"

namespace COMMONS_NS{
    namespace elements{
            enum class text_flags : uint32_t
            {
                default_flag          = 0,
                center_alignment = 1 << 0,
                end_alignment     = 1 << 1
            };
        inline text_flags operator|(text_flags a, text_flags b) {
            return static_cast<text_flags>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
        }

        inline text_flags operator&(text_flags a, text_flags b) {
            return static_cast<text_flags>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
        }

        inline text_flags& operator|=(text_flags& a, text_flags b) {
            a = a | b;
            return a;
        }
        class text : public box {
        public:
            box_type type() const override { return box_type::text; }
            text_flags m_text_flags;
            fvector_type4 limites_iniciais;
            unsigned int m_text_scale;
            std::string m_text_phrase, m_text_font;
            std::string* m_text_phrase_ptr{nullptr};
            text(const std::string phrase,
                    commons::color color,
                    const unsigned int scale = 16,
                    const text_flags flags = text_flags::default_flag,
                    const std::string path_font = "consolas.ttf") : m_text_phrase(phrase)
                , m_text_font(path_font)
                , m_text_scale(scale)
                , m_text_flags(flags)
            {
                m_style.m_width = get_width(phrase);
                m_style.m_height = get_height(phrase);
                m_style.m_color_fundo.a = 1;
                m_shader = (std::make_unique<shader>("image.vert", "text.frag"));
            }
            text(const std::string phrase,
                    const unsigned int scale = 16,
                    const text_flags flags = text_flags::default_flag,
                    const std::string path_font = "consolas.ttf") : m_text_phrase(phrase)
                , m_text_font(path_font)
                , m_text_scale(scale)
                , m_text_flags(flags)
            {
                m_style.m_width = get_width(phrase);
                m_style.m_height = get_height(phrase);
                m_style.m_color_fundo.a = 1;
                m_shader = (std::make_unique<shader>("image.vert", "text.frag"));
            }
            text(std::string* phrase,
                    const unsigned int scale = 16,
                    const text_flags flags = text_flags::default_flag,
                    const std::string path_font = "consolas.ttf") : m_text_phrase_ptr(phrase)
                , m_text_font(path_font)
                , m_text_scale(scale)
                , m_text_flags(flags)
            {
                m_style.m_width = get_width(*phrase);
                m_style.m_color_fundo.a = 1;
                m_shader = (std::make_unique<shader>("image.vert", "text.frag"));
            }

            // retorna a width da linha mais longa dentro de 'phrase'
            float get_width(const std::string& phrase) {
                if (m_text_phrase_ptr) m_text_phrase = *m_text_phrase_ptr;
                auto& caracteres = font_manager::get_instance()
                                       .get(m_text_font, m_text_scale);
                if (caracteres.empty()) {
                     return 0.0f;
                 }

                 float maxLargura = 0.0f;
                 float larguraLinha = 0.0f;

                 for (char32_t c : phrase) {
                     if (c == '\n') {
                         // finaliza a linha atual, atualiza o máximo e reseta
                         maxLargura = std::max(maxLargura, larguraLinha);
                         larguraLinha = 0.0f;
                         continue;
                     }
                    auto it = caracteres.find(c);
                    if (it != caracteres.end()) {
                        const character& ch = it->second;
                        larguraLinha += (ch.avanco >> 6);
                    }
                }
                // compara a última linha (caso não termine em '\n')
                maxLargura = std::max(maxLargura, larguraLinha);
                return maxLargura;
            }

            // retorna a height total considerando quantas linhas exists em 'phrase'
            float get_height(const std::string& phrase) {
                if (m_text_phrase_ptr) m_text_phrase = *m_text_phrase_ptr;
                auto& caracteres = font_manager::get_instance()
                                       .get(m_text_font, m_text_scale);
                if (caracteres.empty()) {
                    return 0.0f;
                }

                // conta quantas quebras de linha exists → número de linhas = quebras + 1
                size_t numQuebras = 0;
                for (char32_t c : phrase) {
                    if (c == '\n') {
                        ++numQuebras;
                    }
                }
                size_t numLinhas = numQuebras + 1;

                // needs de um método no gerenciador para expor a "height de linha"
                // por exemplo: font_manager::get_heightLinha(font, scale)
                float alturaLinha = m_text_scale * 1.4;

                return alturaLinha * static_cast<float>(numLinhas);
            }
            void draw(unsigned int ret_VAO) override {
                limites_iniciais = m_style.m_limits;

                const auto& chs = font_manager::get_instance().get(m_text_font, m_text_scale);
                float y_linha = m_text_scale;
                float x_linha = m_style.m_limits.x;
                if((static_cast<uint32_t>(m_text_flags) & static_cast<uint32_t>(elements::text_flags::center_alignment))!=0) {
                    x_linha += m_style.m_limits.z / 2 - get_width(m_text_phrase)/2;
                }
                if(m_text_phrase_ptr) m_text_phrase = *m_text_phrase_ptr;

                for(const auto& ca : m_text_phrase) {
                    if (chs.empty()) return;
                    if(ca == '\n') {y_linha += m_text_scale; x_linha = m_style.m_limits.x; continue;}
                    character ch;
                    if(chs.find(ca) != chs.end()) ch = chs.at(ca);
                    else continue;

                    float xpos = x_linha + ch.bearing.x;
                    float ypos = limites_iniciais.y - ch.bearing.y + y_linha;
                    float w = ch.size.x;
                    float h = ch.size.y;

                    m_style.m_limits = {xpos, ypos, w, h};
                    m_material.set_texture("text", {ch.id, ""});
                    box::draw(ret_VAO);
                    x_linha += (ch.avanco >> 6);
                }
                if (m_text_phrase_ptr) {
                    m_style.m_width = get_width(m_text_phrase);
                    m_style.m_height = get_height(m_text_phrase);
                }
                m_style.m_limits = {limites_iniciais.x, limites_iniciais.y, m_style.m_width, m_style.m_height};
            };
        };
    } // elements
} // namespace commons
