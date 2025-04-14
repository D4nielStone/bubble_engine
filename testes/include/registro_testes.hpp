/** @copyright 
MIT License
Copyright (c) 2025 Daniel Oliveira

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
 * @file registro_testes.hpp
 * @brief Gerencia a configuração do luabridge para api lua.
 *
 * @see registro_testes.cpp
 */


#pragma once
#include <string>
#include <vector>
#include <functional>

namespace betest
{
    typedef std::function<void()> funcao_teste;
    class registro_testes
    {
        private:
            std::vector<std::pair<std::string, std::pair<std::string, funcao_teste>>> testes;
            std::string prefixo_atual{"[BUBBLE][TESTES]"};
            unsigned int n_falhas {0};
        public:
            registro_testes();
            /// @brief adiciona testes
            /// @param nome 
            /// @param func 
            void adicionar(const std::string& nome, funcao_teste func);
            void classe(const std::string& names_space, const std::string& prefixo);
            void rodarTestes();
    };
}
