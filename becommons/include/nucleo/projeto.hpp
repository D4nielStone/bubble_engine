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
 * \brief Classe projeto
 * \file projeto.hpp
 */

#pragma once
#include <string>
#include <queue>
#include <functional>
#include <rapidjson/document.h>
#include <memory>
#include "fase.hpp"
#include "sistemas/sistema.hpp"
#include "sistemas/sistema_de_fisica.hpp"
#include "sistemas/sistema_de_renderizacao.hpp"
#include "sistemas/sistema_de_codigo.hpp"
#include "sistemas/interface.hpp"

namespace becommons {
    class projeto {
        public:
            // Variáveis de configuração
            std::string m_diretorio, m_nome, m_lancamento, m_icone, m_nome_janela;
            int m_largura, m_altura;
            // \brief Construtor para carregar configuração do projeto.
            // \param diretorio
            projeto(const std::string&);
            ~projeto();
        private:
            // \brief carregar arquivo de configuração e define diretório e outros.
            // \return retorna documento json do projeto
            void analisar();
    };
}
