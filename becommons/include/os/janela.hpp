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
 * @see janela.cpp
 */

#pragma once
#include <functional>
#include <memory>
#include <unordered_map>
#include <glad.h>
#include <GLFW/glfw3.h>
#include "util/vetor2.hpp"

namespace becommons {
	/**
	* @class janela
	* @brief facilita o uso da janela glfw
	* @brief cuida dos callbacks, parametros iniciais etc.
	*/
	class janela {
    public:
        enum class cursor : int {
            mao = 0x00036004,
            i   = 0x00036002,
            seta= 0x00036001,
            re_h= 0x00036005,
            re_v= 0x00036006
        };
        /*enum configuracao : uint8_t {
            padrao          = 0,
            splash_screen =   1
        };

    	janela(const configuracao cfg = padrao);*/
    	janela(const char* nome, fvet2 bounds = fvet2(600, 400), const char* icon_path = nullptr);
    	janela(const char* nome, const bool f, fvet2 bounds = fvet2(600, 400), const char* icon_path = nullptr);
    	~janela();

        bool deveFechar();
        void posicionarCursor                   (double x, double y);
        ivet2 obterTamanhoJanela                ();
    	void poll();
        void definirCursor(const cursor c);
    	void swap();
		void viewport() const;
        std::string nome() const;
    	void nome(const char* novo_nome);

        std::string m_nome{""};
    	ivet2 tamanho;
    	GLFWwindow* window;
    private:
        //configuracao config;
        cursor m_cursor = cursor::seta;
	    cursor m_cursor_antigo = cursor::seta;
	};
}
