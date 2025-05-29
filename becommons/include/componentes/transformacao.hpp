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
#include "util/vetor2.hpp"
#include "util/vetor3.hpp"
#include "util/vetor4.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

namespace BECOMMONS_NS {
	class transformacao : public componente {
    public:
		fvet3 cima, posicao, rotacao, escala;
		fvet3* alvo;
		bool m_usar_alvo, m_alvo_novo{ false };
		glm::mat4 matrizmodelo;
		static constexpr mascara mascara = COMPONENTE_TRANSFORMACAO;
        ~transformacao();
        transformacao(const fvet3& p = fvet3(0.f,0.f,0.f),
			const fvet3& r = fvet3(0.f, 0.f, 0.f), 
			const fvet3& e = fvet3(1.f, 1.f, 1.f));

        bool analizar(const rapidjson::Value&) override;
        bool serializar(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const override;

        glm::mat4 obterMatrizModelo() const;
        fvet3 obterPosicao() const;
        fvet3 obterEscala() const;
        fvet3 obterRotacao() const;
        fvet3 obterAlvo() const;
        fvet3 obterCima() const;
        bool usandoAlvo() const;
        void definirMatrizModelo(const glm::mat4&);
        void definirCima(const fvet3&);
        void definirPosicao(const fvet3&);
        void definirEscala(const fvet3&);
        void definirRotacao(const fvet3&);
        void definirRotacao(const fvet4&);
        void mover(const fvet3&);
        void rotacionar(const fvet3&);
        void escalonar(const fvet3&);
		void olharEntidade(const uint32_t& ent);
		void olharVetor(const fvet3& pos);

		transformacao& operator=(const transformacao& tr);
	};
}
