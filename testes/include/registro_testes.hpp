/** @copyright Copyright (c) 2025 Daniel Oliveira */

#pragma once
#include <string>
#include <stack>
#include <functional>

namespace betest
{
    typedef std::function<void()> funcao_teste;
    class registro_testes
    {
        private:
            std::stack<std::pair<std::string, std::pair<std::string, funcao_teste>>> testes;
            std::string prefixo_atual{"[BUBBLE][TESTES]"};
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