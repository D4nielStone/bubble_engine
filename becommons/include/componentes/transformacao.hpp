/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 *
 * @file transformacao.hpp
 * @brief Calcula a mtriz de modelo para um objeto 3d qualquer
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 *
 * @see transformacao.cpp
 */

#pragma once
#include "namespace.hpp"
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
				matrizmodelo = glm::translate(glm::mat4(1.f), posicao); // Aplica a transla��o
				matrizmodelo *= glm::inverse(glm::lookAt(glm::vec3(0.f), *alvo - posicao, cima));
				matrizmodelo = glm::scale(matrizmodelo, escala);       // Aplica a escala
			}
			else
			{
				matrizmodelo = glm::translate(glm::mat4(1.f), posicao); // Aplica a transla��o
				matrizmodelo = glm::rotate(matrizmodelo, glm::radians(rotacao.x), glm::vec3(1.f, 0.f, 0.f));
				matrizmodelo = glm::rotate(matrizmodelo, glm::radians(rotacao.y), glm::vec3(0.f, 1.f, 0.f));
				matrizmodelo = glm::rotate(matrizmodelo, glm::radians(rotacao.z), glm::vec3(0.f, 0.f, 1.f));
				matrizmodelo = glm::scale(matrizmodelo, escala);       // Aplica a escala
			}
			return matrizmodelo;
		}

		float* obter() { return glm::value_ptr(matrizmodelo); }

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
