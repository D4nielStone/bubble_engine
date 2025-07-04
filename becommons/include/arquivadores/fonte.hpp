/** @copyright 
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
 * @file fonte.hpp
 * Define estruturas básicas para renderização de textos
 */

#pragma once
#include "glad.h"
#include <string>
#include <map>
#include <unordered_map>
#include <ft2build.h>
#include "becommons_namespace.hpp"
#include "util/vetor2.hpp"
#include FT_FREETYPE_H
#include <functional>

/// namespace becommons
namespace BECOMMONS_NS {
    /// @struct carectere
    /// Estrutura base da letra.
    struct caractere
    {
        /// ID da textura
        unsigned int id;
        /// Tamanho do glifo
        vetor2<unsigned int> tamanho;
        /// Deslocamento/Offset vertical
        vetor2<FT_Int>   apoio;
        /// Deslocamento horizontal
        long int avanco;
    };
}
/// Define std::pair<std::string, unsigned int> como t_FonteID.
/// Diretório e id da fonte respectivamente.
typedef std::pair<std::string, unsigned int> t_FonteID;
/// Define std::unordered_map<char32_t, caractere> como t_Caracteres.
typedef std::unordered_map<char32_t, BECOMMONS_NS::caractere> t_Caracteres;
/// Define std::map<t_FonteID, t_Caracteres> como Fontes 
typedef std::unordered_map<t_FonteID, t_Caracteres> t_Fontes;
/// Hash para t_FonteID
namespace std {
    template <>
    struct hash<t_FonteID> {
        std::size_t operator()(const t_FonteID& id) const noexcept {
            std::size_t h1 = std::hash<std::string>{}(id.first);
            std::size_t h2 = std::hash<unsigned int>{}(id.second);
            return h1 ^ (h2 << 1); // Combinação simples
        }
    };
}
namespace BECOMMONS_NS {

    /// @class gerenciadorFontes
    /// Gerencia as fontes caregadas.
    class gerenciadorFontes
    {
    public:
        /// Obtenção da instância global
        static gerenciadorFontes& obterInstancia();

        /// Inicializa o FreeType
        gerenciadorFontes();

        /// Destrutor
        ~gerenciadorFontes();

        /// Carrega uma fonte com a resolução especificada
        void carregar(const std::string& nome_da_fonte, const unsigned int resolucao);
        
        /// Re-inicia o map de fontes carregadas
        static void limparFontes();

        /// Obtém um ponteiro para os caracteres de uma fonte já carregada
        const t_Caracteres& obter(const std::string& nome_da_fonte, const unsigned int resolucao = 20) const;
        
        /// Mapa de fontes carregadas
        t_Fontes fontes;
    private:
        FT_Library ft; ///< biblioteca FreeType
    };
}
/// @see fonte.cpp
