/** @copyright Copyright (c) 2025 Daniel Oliveira */

/**
 * @file projeto.hpp
 * @brief Gerencia configurações do projeto
 */

#pragma once
#include "nucleo/fase.hpp"
#include <string>
#include <rapidjson/document.h>
#include <memory>
#include <unordered_map>
#include "nucleo/sistema.hpp"
#include "nucleo/sistema_de_fisica.hpp"

namespace bubble
{
    struct projeto
    {
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
            
            /// @brief inicia mainloop
            void rodar();
            
            /// @brief cria nova fase e torna atual
            /// @param dir diretório da fase
            void fase(const std::string& dir);
	        std::shared_ptr<bubble::fase> obterFaseAtual();

	        /// @gets para Sistemas
	        sistema_fisica* sfisica();
        private:
            std::unordered_map<std::string, sistema*> sistemas;
            rapidjson::Document obterDoc();
            void criarJanela(rapidjson::Document& doc);
            void iniciarSistemasRuntime(bubble::fase*);
            void iniciarSistemasEditor(bubble::fase*);
            inline static std::shared_ptr<bubble::fase> fase_atual{ nullptr };
    };
    inline projeto* projeto_atual{nullptr}; 
}
