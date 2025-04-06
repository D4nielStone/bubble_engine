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

            /// @brief salva todas as fases do projeto
            void salvarTudo(){};
            /// @brief salva fase pelo nome
            /// @param nome nome da fase
            void salvar(const std::string& nome){};
            /// @brief carrega todas as fases da pasta do projeto
            void carregarTudo(){};
            /// @brief carrega fase pelo nome no diretório do projeto e torna atual
            /// @param nome diretório e nome da fase
            void carregar(const std::string& nome);

            /// @param dir diretório da fase
	        std::shared_ptr<bubble::fase> obterFaseAtual();
	        std::shared_ptr<bubble::fase> obterFase(const std::string& nome){return nullptr;};

            /// @brief adiciona sistema
            void adicionar(const std::string, sistema*);

	        /// @gets para Sistemas
	        sistema_fisica* sfisica();
	        sistema_renderizacao* srender();
	        sistema* obterSistema(const std::string);
        private:
            void criarProjetoVazio(const std::string& path, const char* nome);
            std::unordered_map<std::string, sistema*> sistemas;
            std::unordered_map<std::string, sistema*> sistemas_adicionais;
            std::unordered_map<std::string, std::shared_ptr<bubble::fase>> m_fases;
            rapidjson::Document obterDoc();
            void criarJanela(rapidjson::Document& doc);
            void iniciarSistemas(bubble::fase*);
            std::string fase_atual;
    };
    inline projeto* projeto_atual{nullptr}; 
}
