/** @copyright 
MIT License
Copyright (c) 2025 Daniel Oliveira

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
*/
/**
 * @file texto.hpp
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
