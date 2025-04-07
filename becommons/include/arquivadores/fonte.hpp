/** @copyright Copyright (c) 2025 Daniel Oliveira */

#pragma once
#include <glad/glad.h>
#include "util/vetor2.hpp"
#include <string>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

/**
* @file fonte.hpp
* @brief defini estruturas basicas para renderizacao de textos
*/

namespace bubble
{
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
        void carregar(const std::string& nome_da_fonte, unsigned int resolucao);
        static void limparFontes();

        // Obt�m um ponteiro para os caracteres de uma fonte j� carregada
        const std::map<char32_t, caractere>& obter(const std::string& nome_da_fonte) const;
        std::map<std::string, std::map<char32_t, caractere>> fontes; // Mapa para armazenar as fontes carregadas
    private:
        FT_Library ft; // biblioteca FreeType
    };
}
