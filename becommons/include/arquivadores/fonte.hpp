/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file fonte.hpp
 * @brief Define estruturas básicas para renderização de textos
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 *
 * @see fonte.cpp
 */

#pragma once
#include <glad/glad.h>
#include "util/vetor2.hpp"
#include <string>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

typedef std::pair<std::string, unsigned int> FonteID;
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
        void carregar(const std::string& nome_da_fonte, const unsigned int resolucao);
        static void limparFontes();

        // Obtém um ponteiro para os caracteres de uma fonte já carregada
        const std::map<char32_t, caractere>& obter(const std::string& nome_da_fonte, const unsigned int resolucao = 20) const;
        std::map<FonteID, std::map<char32_t, caractere>> fontes; // Mapa para armazenar as fontes carregadas
    private:
        FT_Library ft; // biblioteca FreeType
    };
}
