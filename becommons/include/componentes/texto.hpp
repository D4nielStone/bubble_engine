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
#include <glm/glm.hpp>
#include <string>
#include "namespace.hpp"
#include "util/cor.hpp"
#include "componente.hpp"

namespace BECOMMONS_NS {
	struct texto : componente
	{
		constexpr static componente::mascara mascara = componente::COMPONENTE_TEXTO;
		std::string frase, fonte = "noto_sans.regular.ttf";
		const float escala;
		vetor2<int> padding{0,0};
		cor m_cor;
        glm::vec3 posicao_referencial; bool pf_ativa = false;// posicao de 3d para 2d
		texto(const std::string& texto, float escala = 1.f) : frase(texto), escala(escala), m_cor({ 1.f, 1.f, 1.f, 1.f }), padding({0, 0}) {}
	};
} 
