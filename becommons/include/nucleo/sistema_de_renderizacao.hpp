/** @copyright Copyright (c) 2025 Daniel Oliveira */

/**
 * @file sistema_de_renderizacao.hpp
 * @brief Gerencia a renderizacao
 */

#pragma once
#include "sistema.hpp"

namespace bubble
{
	class sistemaRenderizacao : public sistema
	{
	public:
		sistemaRenderizacao() = default;
		void atualizar() override;
		void inicializar(bubble::fase* fase_ptr) override;
	};
}