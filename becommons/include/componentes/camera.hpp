/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file camera.hpp
 * @brief Estrutura básica de câmera.
 * @brief Viewport, Matrizes etc.
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 *
 * @see camera.cpp
 */

#pragma once
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <memory>
#include "namespace.hpp"
#include "componente.hpp"
#include "transformacao.hpp"
#include "util/skybox.hpp"
#include "util/cor.hpp"
#include "util/raio.hpp"

namespace BECOMMONS_NS {
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

        skybox* m_skybox{nullptr};
		cor ceu				{0.43F, 0.78F, 0.86F, 1.0F};

		glm::vec3 forward, direita, cima, posicao;
		transformacao* transform{ nullptr };

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

		vetor2<double>* viewport_ptr{ nullptr };
		vetor2<double> viewportFBO{ 400, 400 };

		void desenharFB() const;
        bool analizar(const rapidjson::Value&) override;
        bool serializar(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const override;

        ~camera();
		camera(const bool ortho = false);

		void ativarFB();
		void desativarFB();
		glm::mat4 obtViewMatrix();
		void viewport(const vetor2<double>& viewp);

		glm::mat4 obtProjectionMatrix();

		raio pontoParaRaio(vetor2<double> screenPoint) const;

		glm::vec3 telaParaMundo(const vetor2<double>& screenPoint, float profundidade) const;

        vetor2<int> mundoParaTela(const glm::vec3& mundoPos);
		
		void mover(glm::vec3 pos);
	};
}
