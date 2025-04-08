/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file propriedades.hpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#pragma once
#include "componente.hpp"
#include <string>

namespace bubble
{
	struct propriedades : componente
	{
		
		std::string nome;
		std::string tag;
		bool ativo;
		static constexpr mascara mascara = COMPONENTE_PROPRIEDADES;
		propriedades(const std::string& name = "", const std::string& tag = "", bool active = true)
			: nome(name), tag(tag), ativo(active) {}
	};
}
