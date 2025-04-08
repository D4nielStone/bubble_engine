/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file projeto.hpp
 * @brief Gerencia configurações do projeto
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 *
 * @see projeto.cpp
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
            /// @brief construtor para carregar arquivos do projeto.
            /// @param diretorio 
            projeto(const std::string& diretorio);
            /// @brief construtor para iniciar janela sem carregar arquivos da pasta do projeto.
            projeto();
            ~projeto();

            /// @brief inicia mainloop
            void rodar();

            /// @brief salva todas as fases do projeto
            void salvarFases();
            /// @brief salva fase pelo nome
            /// @param nome nome da fase
            void salvarFase(const std::string& nome);
            /// @brief carrega todas as fases da pasta do projeto
            void carregarFases(){};
            /// @brief carrega fase pelo nome no diretório do projeto e torna atual
            /// @param nome diretório e nome da fase
            void carregarFase(const std::string& nome);
            /// @brief carregar arquivo de configuração e define diretório e outros.
            /// @return retorna documento json do projeto
            rapidjson::Document carregarProjeto();
            /// @brief salva todas as fases e a configuração do projeto
            void salvarProjeto(){};
            /// @brief obtenção de uma fase dentro da lista da fases carregadas
            /// @param nome diretório da fase
	        /// @return ponteiro inteligente da fase
	        std::shared_ptr<bubble::fase> obterFase(const std::string& nome){return nullptr;};
	        /// @brief obtenção da fase atualmente em execução
	        /// @return retorna ponteiro da fase atual
	        std::shared_ptr<bubble::fase> obterFaseAtual();

            /// @brief adiciona sistema
            /// @param nome Nome do sistema
            /// @param s Ponteiro do Sistema 
            void adicionar(const std::string nome, sistema* s);

	        /// @gets para Sistemas
	        sistema_fisica* sfisica();
	        sistema_renderizacao* srender();
	        sistema* obterSistema(const std::string);
        private:
            /// @brief cria projeto com cubo, luz direcional e código de rotação.
            void criarProjetoVazio(const std::string& path, const char* nome);
            /// @brief sistemas padrão
            std::unordered_map<std::string, sistema*> sistemas;
            /// @brief sistemas adicionais
            std::unordered_map<std::string, sistema*> sistemas_adicionais;
            /// @brief vetor de fases carregadas
            std::unordered_map<std::string, std::shared_ptr<bubble::fase>> m_fases;
            /// @brief cria janela glfw e inicia contexto opengl
            void criarJanela(rapidjson::Document& doc);
            /// @brief adiciona sistemas padrão como interface, renderização e física
            void iniciarSistemas(bubble::fase*);
            /// @brief id para fase atual
            std::string fase_atual;
    };
    /// @brief Instância global do projeto atual
    inline projeto* projeto_atual{nullptr}; 
}
