/** @copyright Copyright (c) 2025 Daniel Oliveira */

/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file registro_testes.hpp
 * @brief Gerencia a configuração do luabridge para api lua.
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 *
 * @see .cpp
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
