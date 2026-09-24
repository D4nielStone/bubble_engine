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
 * @file font.cpp
 */


#include <filesystem>
#include "loaders/font.hpp"
#include "assets/fonts_in_memory.hpp"

namespace COMMONS_NS
{
    std::unordered_map<std::string, std::pair<const unsigned char*, unsigned int>> fonts_memory
    {
        {"consolas.ttf", std::pair(Consolas_ttf, Consolas_ttf_len)},
        {"consolai.ttf", std::pair(consolai_ttf, consolai_ttf_len)},
        {"consola.ttf", std::pair(Consola_ttf, Consola_ttf_len)},
        {"consolaz.ttf", std::pair(consolaz_ttf, consolaz_ttf_len)},
        {"noto_sans.italic.ttf", std::pair(noto_sans_italic_ttf, noto_sans_italic_ttf_len)},
        {"noto_sans.bold.ttf", std::pair(noto_sans_bold_ttf, noto_sans_bold_ttf_len)},
        {"noto_sans.regular.ttf", std::pair(noto_sans_regular_ttf, noto_sans_regular_ttf_len)}
    };

    font_manager::font_manager()
    {
        if (FT_Init_FreeType(&ft))
        {
            throw std::runtime_error("Erro ao setup FreeType");
        }
    }

    font_manager& font_manager::get_instance()
    {
        static font_manager instance;
        return instance;
    }

    font_manager::~font_manager()
    {
        fonts.clear();
        FT_Done_FreeType(ft);
    }

    void font_manager::limparFontes()
    {
        font_manager::get_instance().fonts.clear();
    }

    void font_manager::load(const std::string& font_name, const unsigned int resolucao)
    {
        if (fonts.find({font_name, resolucao}) != fonts.end())
        {
            return; // Fonte já carregada
        }

        FT_Face face{};

        if (fonts_memory.find(std::filesystem::path(font_name).filename().string()) != fonts_memory.end())
        {
            if (FT_New_Memory_Face(ft, fonts_memory.at((std::filesystem::path(font_name).filename().string())).first, fonts_memory.at((std::filesystem::path(font_name).filename().string())).second, 0, &face))
            {
                throw std::runtime_error("Erro ao load a font: " + font_name);
            }
        }
        else
            if (std::filesystem::exists(font_name)) {
                if (FT_New_Face(ft, font_name.c_str(), fonts.size(), &face)) {
                    throw std::runtime_error("Erro ao load a font: " + font_name);
                }
            }

        FT_Set_Pixel_Sizes(face, 0, resolucao);

        t_Caracteres caracteres;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        for (unsigned int c = 0; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                throw std::runtime_error("Erro ao load letra: " + c);
                continue;
            }

            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            character character = {
                texture,
                vector2<unsigned int>{face->glyph->bitmap.width, face->glyph->bitmap.rows},
                vector2<FT_Int>{face->glyph->bitmap_left, face->glyph->bitmap_top},
                face->glyph->advance.x
            };
            caracteres.emplace(c, character);
        }

        fonts.emplace(font_id(font_name, resolucao), caracteres);

        FT_Done_Face(face);
    }

    const t_Caracteres& font_manager::get(const std::string& font_name, const unsigned int resolucao) const
    {
        auto it = fonts.find({font_name, resolucao});
        if (it != fonts.end())
        {
            return it->second;
        }
        get_instance().load(font_name, resolucao);
        auto it_ = fonts.find({font_name, resolucao});
            return it_->second;

    }
}
