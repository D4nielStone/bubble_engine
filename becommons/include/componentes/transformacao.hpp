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
 *
 * @file transformacao.hpp
 * @brief Calcula a mtriz de modelo para um objeto 3d qualquer
 * @see transformacao.cpp
 */

#pragma once
#include "becommons_namespace.hpp"
#include "componente.hpp"
#include "util/vetor3.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

namespace BECOMMONS_NS {
	class transformacao : public componente
	{
    public:
		glm::vec3 cima{ 0,1,0 };
		glm::vec3 posicao{}, rotacao{}, escala{};
		glm::vec3* alvo{ nullptr };
		glm::mat4 matrizmodelo;
		static constexpr mascara mascara = COMPONENTE_TRANSFORMACAO;

        bool analizar(const rapidjson::Value&) override;
        bool serializar(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const override;
		transformacao(const fvet3& p = fvet3(0.f,0.f,0.f),
			const fvet3& r = fvet3(0.f, 90.f, 0.f), 
			const fvet3& e = fvet3(1.f, 1.f, 1.f)) :
			posicao({ p.x,p.y,p.z }),
			rotacao({ r.x,r.y,r.z }),
			escala({ e.x,e.y,e.z })
		{
		}

		glm::mat4 calcular()
		{
			if (alvo)
			{
				matrizmodelo = glm::translate(glm::mat4(1.f), posicao); // Aplica a translação
				matrizmodelo *= glm::inverse(glm::lookAt(glm::vec3(0.f), *alvo - posicao, cima));
				matrizmodelo = glm::scale(matrizmodelo, escala);       // Aplica a escala
			}
			else
			{
				matrizmodelo = glm::translate(glm::mat4(1.f), posicao); // Aplica a translação
				matrizmodelo = glm::rotate(matrizmodelo, glm::radians(rotacao.x), glm::vec3(1.f, 0.f, 0.f));
				matrizmodelo = glm::rotate(matrizmodelo, glm::radians(rotacao.y), glm::vec3(0.f, 1.f, 0.f));
				matrizmodelo = glm::rotate(matrizmodelo, glm::radians(rotacao.z), glm::vec3(0.f, 0.f, 1.f));
				matrizmodelo = glm::scale(matrizmodelo, escala);       // Aplica a escala
			}
			return matrizmodelo;
		}

        glm::mat4 obter() { return matrizmodelo; }

		transformacao& operator=(const transformacao& tr)
		{
			this->posicao = tr.posicao;
			this->rotacao = tr.rotacao;
			this->escala = tr.escala;
			return *this;
		}

		void apontarEntidade(const uint32_t& ent);
		void apontarV3(const glm::vec3& pos);
	};
}
