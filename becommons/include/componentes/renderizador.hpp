/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
#include "componente.hpp"
#include "arquivadores/modelo.hpp"

namespace bubble
{
	struct renderizador : componente
	{
		bubble::modelo* modelo;
		static constexpr mascara mascara = COMPONENTE_RENDER;

		renderizador(bubble::modelo* malha) : modelo(malha)
		{
		}
		renderizador(const char* diretorio = "") {};
		~renderizador()
		{
			for(auto& malha : modelo->malhas)
			{
				malha.descarregar();
			}
			delete modelo;
		}
	};
}