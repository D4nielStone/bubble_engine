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
 * @file sistema.hpp
 */
#pragma once
#include "becommons_namespace.hpp"

namespace BECOMMONS_NS {
    /**
     * @struct sistema
     * Classe base para sistemas do loop principal
     */
    struct sistema {
        /**
         * Construtor virtual da classe Sistema
         * @param nome Nome do sistema
         */
        sistema() = default;

        /**
         * @brief Destrutor virtual para garantir a destruição adequada das subclasses
         */
        virtual ~sistema() = default;

        /**
         * @brief Inicializa o sistema
         */
        virtual void inicializar() = 0;

        /**
         * @brief Atualiza o sistema dentro do loop principal
         * @param deltaTime Tempo desde a última atualização (em segundos)
         */
        virtual void atualizar() = 0;
    };
}
