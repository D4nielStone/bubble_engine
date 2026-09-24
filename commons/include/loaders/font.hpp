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
 * @file font.hpp
 * Define estruturas básicas para renderização de textos
 */

#pragma once
#include "glad.h"
#include <string>
#include <map>
#include <unordered_map>
#include <ft2build.h>
#include "commons_namespace.hpp"
#include "util/vector2.hpp"
#include FT_FREETYPE_H
#include <functional>

/// namespace commons
namespace COMMONS_NS {
    /// @struct character
    /// Estrutura base da letra.
    struct character
    {
        /// ID da texture
        unsigned int id;
        /// Tamanho do glifo
        vector2<unsigned int> size;
        /// Deslocamento/Offset vertical
        vector2<FT_Int>   bearing;
        /// Deslocamento horizontal
        long int avanco;
    };
}
/// Define std::pair<std::string, unsigned int> como font_id.
/// Diretório e id da font respectivamente.
typedef std::pair<std::string, unsigned int> font_id;
/// Define std::unordered_map<char32_t, character> como t_Caracteres.
typedef std::unordered_map<char32_t, COMMONS_NS::character> t_Caracteres;
/// Define std::map<font_id, t_Caracteres> como Fontes
typedef std::unordered_map<font_id, t_Caracteres> t_Fontes;
/// Hash para font_id
namespace std {
    template <>
    struct hash<font_id> {
        std::size_t operator()(const font_id& id) const noexcept {
            std::size_t h1 = std::hash<std::string>{}(id.first);
            std::size_t h2 = std::hash<unsigned int>{}(id.second);
            return h1 ^ (h2 << 1); // Combinação simples
        }
    };
}
namespace COMMONS_NS {

    /// @class font_manager
    /// Gerencia as fonts caregadas.
    class font_manager
    {
    public:
        /// Obtenção da instância global
        static font_manager& get_instance();

        /// Inicializa o FreeType
        font_manager();

        /// Destrutor
        ~font_manager();

        /// Carrega uma font com a resolução especificada
        void load(const std::string& font_name, const unsigned int resolucao);

        /// Re-inicia o map de fonts loaded
        static void limparFontes();

        /// Obtém um ponteiro para os caracteres de uma font já carregada
        const t_Caracteres& get(const std::string& font_name, const unsigned int resolucao = 20) const;

        /// Mapa de fonts loaded
        t_Fontes fonts;
    private:
        FT_Library ft; ///< biblioteca FreeType
    };
}
/// @see font.cpp
