/** @copyright Copyright (c) 2025 Daniel Oliveira */

/**
 * @file projeto.hpp
 * @brief Gerencia configurações do projeto
 */

#pragma once
#include "string"
#include "nucleo/fase.hpp"

namespace bubble
{
    class projeto
    {
        public:
            std::string diretorioDoProjeto;
            /// @brief construtor
            /// @param diretorio 
            inline static bubble::fase* fase_atual{ nullptr };
            projeto() = default;
            projeto(const std::string& diretorio);
            void rodar();
            void fase(const std::string& nome);
            bubble::fase* obterFaseAtual();
    };
    inline projeto* projeto_atual{nullptr}; 
}