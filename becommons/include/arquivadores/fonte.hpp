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
 * @file fonte.hpp
 * Define estruturas básicas para renderização de textos
 */

#pragma once
#include <glad/glad.h>
#include <string>
#include <map>
#include <unordered_map>
#include <ft2build.h>
#include "namespace.hpp"
#include "util/vetor2.hpp"

#include FT_FREETYPE_H

/// Define std::pair<std::string, unsigned int> como FonteID.
/// Diretório e id da fonte respectivamente.
typedef std::pair<std::string, unsigned int> FonteID;
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
        const std::map<char32_t, caractere>& obter(const std::string& nome_da_fonte, const unsigned int resolucao = 20) const;
        
        /// Mapa de fontes carregadas
        std::map<FonteID, std::map<char32_t, caractere>> fontes;
    private:
        FT_Library ft; ///< biblioteca FreeType
    };
}
/// @see fonte.cpp
