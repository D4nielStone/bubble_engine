/** \copyright
 * MIT License
 * Copyright (c) 2025 Daniel Oliveira
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * \brief Gerencia as fases
 * \file levelmanager.hpp
 */

#pragma once
#include "fase.hpp"
#include "projeto.hpp"
#include <memory>

namespace becommons {
    class levelmanager {
    /* Level Management */
    private:
        projeto m_projeto;
        // \brief id para fase atual
        std::string fase_atual;
        // \brief vetor de fases carregadas
        std::unordered_map<std::string, std::shared_ptr<fase>> m_fases;
    public:
        levelmanager(projeto*);
        // \brief obtenção de uma fase dentro da lista da fases carregadas
        // \param nome diretório da fase
	    // \return ponteiro inteligente da fase
	    std::shared_ptr<fase> obter(const std::string& nome);
        // \brief salva fase pelo nome
        // \param nome nome da fase
        void salvar(const std::string& nome);
        // \brief carrega fase pelo nome no diretório do projeto
        // \param nome diretório e nome da fase
        void carregar(const std::string& nome);
        // \brief carrega todas as fases da pasta do projeto
        void carregarTudo();
        // \brief salva todas as fases do projeto
        void salvarTudo();
	    // \brief obtenção da fase atualmente em execução
	    // \return retorna ponteiro da fase atual
	    std::shared_ptr<fase> obterFaseAtual();
    };
}
