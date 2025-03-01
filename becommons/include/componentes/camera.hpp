/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
#include "componente.hpp"
#include "util/raio.hpp"
#include "util/vetor2.hpp"
#include "util/vetor3.hpp"
#include "util/vetor4.hpp"
#include "util/cor.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "transformacao.hpp"
#include <memory>

namespace bubble
{
	/**
	 * @struct camera
	 * @brief componente que facilita a manipuacao da matriz de visualizacao e projeco
	 */
	struct camera : componente
	{
		/**
		 * @enum configCamera
		 * @brief configuracao da camera;
		 */

		cor ceu				{0.43F, 0.78F, 0.86F, 1.0F};

		glm::vec3 forward, direita, cima, posicao;
		std::shared_ptr<bubble::transformacao> transform{ nullptr };

		glm::mat4 viewMatrix = glm::mat4(1.f), projMatriz = glm::mat4(1.f);

		unsigned int fbo, textura, rbo;

		float fov			{ 75.f };
		float aspecto		{ 0.f };
		float corte_curto	{ 0.1f }; 
		float corte_longo	{ 300.f };
		float escala		{ 5.f };

		float left, top, bottom, right;

		bool flag_orth		{ false };
		bool flag_fb		{ false };

		static constexpr mascara mascara = COMPONENTE_CAM;

		vet2* viewport_ptr{ nullptr };
		vet2 viewportFBO{ 400, 400 };

		void desenharFB() const;

		~camera();
		camera() = default;
		camera(const bool ortho);

		void ativarFB();
		void desativarFB();
		glm::mat4 obtViewMatrix();
		void viewport(const vet2& viewp);

		glm::mat4 obtProjectionMatrix();

		raio pontoParaRaio(vet2 screenPoint) const;

		glm::vec3 telaParaMundo(const vet2& screenPoint, float profundidade) const;
		
		/// @brief 
		/// @param pos 
		void mover(glm::vec3& pos);
	};
}