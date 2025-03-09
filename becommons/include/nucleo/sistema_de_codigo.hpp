/** @copyright Copyright(c) 2024 Daniel Oliveira */

/**
 * @file sistema_de_codigo.hpp
 * @brief Gerencia a api lua
 */

#pragma once
#include "sistema.hpp"
#include <thread>
#include <atomic>

namespace bubble
{
	class sistema_codigo : public sistema
	{
	private:
		std::atomic<bool> rodando{ false }; // Controle da thread
		std::thread codigoThread;
	public:
		sistema_codigo() = default;
		~sistema_codigo();
		void atualizar() override;
		void inicializar(bubble::fase* fase_ptr) override;
		void iniciarThread();
		void pararThread();
	};
}