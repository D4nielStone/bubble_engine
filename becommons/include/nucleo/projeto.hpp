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
 * @file projeto.hpp
 * @brief Gerencia configurações do projeto
 *
 * @see projeto.cpp
 */

#pragma once
#include <string>
#include <rapidjson/document.h>
#include <memory>
#include "namespace.hpp"
#include "fase.hpp"
#include "sistema.hpp"
#include "sistema_de_fisica.hpp"
#include "sistema_de_renderizacao.hpp"

namespace BECOMMONS_NS {
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
	        std::shared_ptr<fase> obterFase(const std::string& nome){return nullptr;};
	        /// @brief obtenção da fase atualmente em execução
	        /// @return retorna ponteiro da fase atual
	        std::shared_ptr<fase> obterFaseAtual();

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
            std::unordered_map<std::string, std::shared_ptr<fase>> m_fases;
            /// @brief cria janela glfw e inicia contexto opengl
            void criarJanela(rapidjson::Document& doc);
            /// @brief adiciona sistemas padrão como interface, renderização e física
            void iniciarSistemas(fase*);
            /// @brief id para fase atual
            std::string fase_atual;
    };
    /// @brief Instância global do projeto atual
    inline projeto* projeto_atual{nullptr}; 
}
