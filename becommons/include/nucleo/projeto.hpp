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
#include "nucleo/sistema.hpp"
#include "nucleo/sistema_de_fisica.hpp"
#include "nucleo/sistema_de_renderizacao.hpp"

namespace bubble
{
    struct projeto
    {
            std::string diretorioDoProjeto;
            /// @brief construtor
            /// @param diretorio 
            projeto(const std::string& diretorio);
            projeto() = default;
            ~projeto();

            /// @brief inicia mainloop
            void rodar();
            
            /// @brief cria nova fase e torna atual
            /// @param dir diretório da fase
            void fase(const std::string& dir);
	        std::shared_ptr<bubble::fase> obterFaseAtual();

            void adicionar(const std::string, sistema*);

	        /// @gets para Sistemas
	        sistema_fisica* sfisica();
	        sistema_renderizacao* srender();
	        sistema* obterSistema(const std::string);
        private:
            void criarProjetoVazio(const std::string& path, const char* nome);
            std::unordered_map<std::string, sistema*> sistemas;
            std::unordered_map<std::string, sistema*> sistemas_adicionais;
            rapidjson::Document obterDoc();
            void criarJanela(rapidjson::Document& doc);
            void iniciarSistemas(bubble::fase*);
            inline static std::shared_ptr<bubble::fase> fase_atual{ nullptr };
    };
    inline projeto* projeto_atual{nullptr}; 
}
