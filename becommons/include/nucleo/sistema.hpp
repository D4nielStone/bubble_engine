/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file sistema.hpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 *
 */

#pragma once
#include "namespace.hpp"
#include "fase.hpp"
#include "registro.hpp"

 /**
  * @class Sistema
  * @brief Classe base para sistemas do loop principal
  */

namespace BECOMMONS_NS
{
    class fase;
    class sistema {
    public:
        /**
         * @brief Construtor virtual da classe Sistema
         * @param nome Nome do sistema
         */
        sistema() = default;

        /**
         * @brief Destrutor virtual para garantir a destrui��o adequada das subclasses
         */
        virtual ~sistema() = default;

        /**
         * @brief Inicializa o sistema
         */
        virtual void inicializar(fase* fase_ptr);

        /**
         * @brief Atualiza o sistema dentro do loop principal
         * @param deltaTime Tempo desde a �ltima atualiza��o (em segundos)
         */
        virtual void atualizar() = 0;

    protected:
        fase* m_fase;
        registro* reg;
    };
}
