/** @copyright 
 * MIT License
 * Copyright (c) 2025 Daniel Oliveira
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 * @see camera.cpp
 */

#pragma once
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <memory>
#include <glad.h>
#include "componente.hpp"
#include "transformacao.hpp"
#include "util/skybox.hpp"
#include "util/vetor3.hpp"
#include "util/cor.hpp"
#include "util/raio.hpp"

namespace becommons {
	/**
	 * @class camera
	 * @brief componente que facilita a manipuacao da matriz de visualizacao e projeco
	 */
	class camera : public componente {
    public:
		/**
		 * @enum configCamera
		 * @brief configuracao da camera;
		 */

        skybox* m_skybox{nullptr};
        bool m_use_skybox { false };
		cor ceu				{0.43F, 0.78F, 0.86F, 1.0F};
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

		fvet4* viewport_ptr{ nullptr };

		void desenharFB();
		void limparFB() const;
        bool analizar(const rapidjson::Value&) override;
        bool serializar(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const override;

        ~camera();
		camera(const bool ortho = false);

		void ativarFB();
		void desativarFB();
		glm::mat4 obtViewMatrix();

		glm::mat4 obtProjectionMatrix();

		raio pontoParaRaio(const ivet2& screenPoint) const;
		fvet3 telaParaMundo(const ivet2&, float) const;
        ivet3 mundoParaTela(const fvet3&);
    private:
        GLint prevViewport[4];
        GLint prevFBO = 0;
	};
}
