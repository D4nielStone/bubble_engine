/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file sistema_de_interface.hpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 *
 * @see sistema_de_interface.cpp
 */

#pragma once
#include "sistema.hpp"
#include "arquivadores/shader.hpp"
#include "componentes/texto.hpp"
#include "componentes/imagem.hpp"

namespace bubble
{
	class sistema_interface : public sistema
	{
	public:
        inline static glm::mat4 projection{1.f};
        inline static bubble::shader* shader_texto{ nullptr };
        inline static bubble::shader* shader_imagem{ nullptr };
		sistema_interface() = default;
		~sistema_interface() override;
		void atualizar() override;
		void inicializar(bubble::fase* fase_ptr) override;
		static void desenharTexto(shader& s, const texto& t);
		static void desenharImagem(shader& s, const imagem& img);
	private:
		inline static unsigned int text_VAO = 0, text_VBO = 0;
		inline static unsigned int img_VAO= 0, img_VBO = 0, img_EBO = 0;
	};
}
