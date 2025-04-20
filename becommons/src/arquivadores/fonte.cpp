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
 * @file fonte.cpp
 */


#include <filesystem>
#include "arquivadores/fonte.hpp"
#include "assets/fontes_na_memoria.hpp"

namespace BECOMMONS_NS
{
    std::unordered_map<std::string, std::pair<const unsigned char*, unsigned int>> fontes_memoria
    {
        {"consolas.ttf", std::pair(Consolas_ttf, Consolas_ttf_len)},
        {"consolai.ttf", std::pair(consolai_ttf, consolai_ttf_len)},
        {"consola.ttf", std::pair(Consola_ttf, Consola_ttf_len)},
        {"consolaz.ttf", std::pair(consolaz_ttf, consolaz_ttf_len)},
        {"noto_sans.italic.ttf", std::pair(noto_sans_italic_ttf, noto_sans_italic_ttf_len)},
        {"noto_sans.bold.ttf", std::pair(noto_sans_bold_ttf, noto_sans_bold_ttf_len)},
        {"noto_sans.regular.ttf", std::pair(noto_sans_regular_ttf, noto_sans_regular_ttf_len)}
    };

    gerenciadorFontes::gerenciadorFontes()
    {
        if (FT_Init_FreeType(&ft))
        {
            throw std::runtime_error("Erro ao inicializar FreeType");
        }
    }

    gerenciadorFontes& gerenciadorFontes::obterInstancia()
    {
        static gerenciadorFontes instance;
        return instance;
    }

    gerenciadorFontes::~gerenciadorFontes()
    {
        fontes.clear();
        FT_Done_FreeType(ft);
    }

    void gerenciadorFontes::limparFontes()
    {
        gerenciadorFontes::obterInstancia().fontes.clear();
    }

    void gerenciadorFontes::carregar(const std::string& nome_da_fonte, const unsigned int resolucao)
    {
        if (fontes.find({nome_da_fonte, resolucao}) != fontes.end())
        {
            return; // Fonte já carregada
        }

        FT_Face face{};

        if (fontes_memoria.find(std::filesystem::path(nome_da_fonte).filename().string()) != fontes_memoria.end())
        {
            if (FT_New_Memory_Face(ft, fontes_memoria.at((std::filesystem::path(nome_da_fonte).filename().string())).first, fontes_memoria.at((std::filesystem::path(nome_da_fonte).filename().string())).second, 0, &face))
            {
                throw std::runtime_error("Erro ao carregar a fonte: " + nome_da_fonte);
            }
        }
        else
            if (std::filesystem::exists(nome_da_fonte)) {
                if (FT_New_Face(ft, nome_da_fonte.c_str(), fontes.size(), &face)) {
                    throw std::runtime_error("Erro ao carregar a fonte: " + nome_da_fonte);
                }
            }

        FT_Set_Pixel_Sizes(face, 0, resolucao);

        t_Caracteres caracteres;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        for (unsigned int c = 0; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                throw std::runtime_error("Erro ao carregar letra: " + c);
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

            caractere character = {
                texture,
                vetor2<unsigned int>{face->glyph->bitmap.width, face->glyph->bitmap.rows},
                vetor2<FT_Int>{face->glyph->bitmap_left, face->glyph->bitmap_top},
                face->glyph->advance.x
            };
            caracteres.emplace(c, character);
        }

        fontes.emplace(t_FonteID(nome_da_fonte, resolucao), caracteres);

        FT_Done_Face(face);
    }

    const t_Caracteres& gerenciadorFontes::obter(const std::string& nome_da_fonte, const unsigned int resolucao) const
    {
        auto it = fontes.find({nome_da_fonte, resolucao});
        if (it != fontes.end())
        {
            return it->second;
        }
        obterInstancia().carregar(nome_da_fonte, resolucao);
        auto it_ = fontes.find({nome_da_fonte, resolucao});
            return it_->second;
        
    }
}
