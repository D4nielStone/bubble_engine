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
 * @file sistema_de_interface.hpp
 *
 * @see sistema_de_interface.cpp
 */

#pragma once
#include "sistema.hpp"
#include "arquivadores/shader.hpp"
#include "componentes/texto.hpp"
#include "componentes/imagem.hpp"

namespace BECOMMONS_NS {
	class sistema_interface : public sistema
	{
	public:
        inline static glm::mat4 projection{1.f};
        inline static shader* shader_texto{ nullptr };
        inline static shader* shader_imagem{ nullptr };
		sistema_interface() = default;
		~sistema_interface() override;
		void atualizar() override;
		void inicializar(fase* fase_ptr) override;
		static void desenharTexto(shader& s, const texto& t);
		static void desenharImagem(shader& s, const imagem& img);
	private:
		inline static unsigned int text_VAO = 0, text_VBO = 0;
		inline static unsigned int img_VAO= 0, img_VBO = 0, img_EBO = 0;
	};
}
