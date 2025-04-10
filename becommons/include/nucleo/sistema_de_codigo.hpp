/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file sistema_de_codigo.hpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 *
 * @see sistema_de_codigo.cpp
 */

#pragma once
#include "sistema.hpp"
#include <thread>
#include <atomic>

namespace BECOMMONS_NS {
	class sistema_codigo : public sistema
	{
	private:
		std::atomic<bool> rodando{ false }; // Controle da thread
		std::thread codigoThread;
	public:
		sistema_codigo() = default;
		~sistema_codigo();
		void atualizar() override;
		void inicializar(fase* fase_ptr) override;
		void iniciarThread();
		void pararThread();
	};
}
