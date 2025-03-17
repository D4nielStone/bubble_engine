/** @copyright Copyright (c) 2025 Daniel Oliveira */

/**
 * @file projeto.hpp
 * @brief Gerencia configurações do projeto
 */

#pragma once
#include "string"
#include "nucleo/fase.hpp"
#include <memory>

namespace bubble
{
    class projeto
    {
        public:
            std::string diretorioDoProjeto;
            /// @brief construtor
            /// @param diretorio 
            inline static std::shared_ptr<bubble::fase> fase_atual{ nullptr };
            projeto() = default;
            projeto(const std::string& diretorio);
            void rodar();
            void fase(const std::string& nome);
	    std::shared_ptr<bubble::fase> obterFaseAtual();
    };
    inline projeto* projeto_atual{nullptr}; 
}
