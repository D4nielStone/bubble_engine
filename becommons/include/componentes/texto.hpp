/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file texto.hpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */


#pragma once
#include "componente.hpp"
#include "util/cor.hpp"
#include <glm/glm.hpp>
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
        glm::vec3 posicao_referencial; bool pf_ativa = false;// posicao de 3d para 2d
		texto(const std::string& texto, float escala = 1.f) : frase(texto), escala(escala), cor({ 1.f, 1.f, 1.f, 1.f }), padding({0, 0}) {}
	};
} 
