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
 * @file fonte.hpp
 * @brief Define estruturas básicas para renderização de textos
 *
 * @see fonte.cpp
 */

#pragma once
#include <glad/glad.h>
#include <string>
#include <map>
#include <ft2build.h>
#include "namespace.hpp"
#include "util/vetor2.hpp"

#include FT_FREETYPE_H

typedef std::pair<std::string, unsigned int> FonteID;
namespace BECOMMONS_NS {
    // Estrutura base da letra
    struct caractere
    {
        unsigned int id;        // ID da textura
        vetor2<unsigned int> tamanho;    // tamanho do glifo
        vetor2<FT_Int>   apoio;    // offset
        long int avanco;    // Offset para o avanco do proximo glifo
    };


    class gerenciadorFontes
    {
    public:
        static gerenciadorFontes& obterInstancia();

        // Inicializa o FreeType
        gerenciadorFontes();

        // Destrutor
        ~gerenciadorFontes();

        // Carrega uma fonte com a resolu��o especificada
        void carregar(const std::string& nome_da_fonte, const unsigned int resolucao);
        static void limparFontes();

        // Obtém um ponteiro para os caracteres de uma fonte já carregada
        const std::map<char32_t, caractere>& obter(const std::string& nome_da_fonte, const unsigned int resolucao = 20) const;
        std::map<FonteID, std::map<char32_t, caractere>> fontes; // Mapa para armazenar as fontes carregadas
    private:
        FT_Library ft; // biblioteca FreeType
    };
}
