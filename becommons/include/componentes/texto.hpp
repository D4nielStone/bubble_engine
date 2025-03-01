/** @copyright Copyright (c) 2025 Daniel Oliveira */

#pragma once
#include "componente.hpp"
#include "util/cor.hpp"
#include "util/vetor2.hpp"
#include <string>

namespace bubble
{
	struct texto : componente
	{
		constexpr static componente::mascara mascara = componente::COMPONENTE_TEXTO;
		std::string frase, fonte = "noto_sans.regular.ttf";
		const float escala;
		bubble::vetor2<int> padding{0,0};
		bubble::cor cor;
		texto(const std::string& texto, float escala = 1.f) : frase(texto), escala(escala), cor({ 1.f, 1.f, 1.f, 1.f }), padding({0, 0}) {}
	};
} 