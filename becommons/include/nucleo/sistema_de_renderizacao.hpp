/** @copyright Copyright (c) 2025 Daniel Oliveira */

/**
 * @file sistema_de_renderizacao.hpp
 * @brief Gerencia a renderizacao
 */

#pragma once
#include "sistema.hpp"

namespace bubble
{
	class sistema_renderizacao : public sistema
	{
	public:
		sistema_renderizacao() = default;
		void atualizar() override;
		void inicializar(bubble::fase* fase_ptr) override;
	};
}