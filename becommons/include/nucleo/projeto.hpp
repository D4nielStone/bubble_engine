/** @copyright Copyright (c) 2025 Daniel Oliveira */

/**
 * @file projeto.hpp
 * @brief Gerencia configurações do projeto
 */

#pragma once
#include <string>
#include "nucleo/fase.hpp"
#include <memory>

namespace bubble
{
    class projeto
    {
        public:
            /// @brief modo de execução do projeto
            enum modoDeExecucao
            {
                editor,
                runtime
            };

            std::string diretorioDoProjeto;
            /// @brief construtor
            /// @param diretorio 
            projeto(const std::string& diretorio, const modoDeExecucao = runtime);
            projeto() = default;

            void rodar();
            
            /// @brief cria nova fase e torna atual
            /// @param nome nome da fase
            void fase(const std::string& nome);
	        std::shared_ptr<bubble::fase> obterFaseAtual();
        private:
            inline static std::shared_ptr<bubble::fase> fase_atual{ nullptr };
    };
    inline projeto* projeto_atual{nullptr}; 
}
