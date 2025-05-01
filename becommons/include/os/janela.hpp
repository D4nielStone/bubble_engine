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
 * @file janela.hpp
 * @brief Inicia e gerencia janela glfw
 *
 * @see janela.cpp
 */

#pragma once
#include <functional>
#include <memory>
#include <unordered_map>
#include "glad.h"
#include <GLFW/glfw3.h>
#include "namespace.hpp"
#include "tempo.hpp"
#include "util/vetor2.hpp"
#include "inputs/inputs.hpp"

namespace BECOMMONS_NS {
	/**
	* @struct janela
	* @brief facilita o uso da janela glfw
	* @brief cuida dos callbacks, parametros iniciais etc.
	*/
	struct janela
	{
        enum class cursor : int
        {
            mao = 0x00036004,
            i   = 0x00036002,
            seta= 0x00036001,
            re_h= 0x00036005,
            re_v= 0x00036006
        };
        private:
    	    cursor m_cursor = cursor::seta;
	        cursor m_cursor_antigo = cursor::seta;
	        static void modoLegado();
        public:
            static janela& obterInstancia();
            static void gerarInstancia(const char* nome, dvet2 bounds = dvet2(600, 400), const char* icon_path = nullptr);
    		static void gerarInstancia(const char* nome, const bool f, dvet2 bounds = dvet2(600, 400), const char* icon_path = nullptr);

    		tempo m_tempo;
            std::string m_nome{""};
    		dvet2 tamanho;
    		GLFWwindow* window;
    		inputs m_inputs;
    		janela(const char* nome, dvet2 bounds = dvet2(600, 400), const char* icon_path = nullptr);
    		janela(const char* nome, const bool f, dvet2 bounds = dvet2(600, 400), const char* icon_path = nullptr);
    		~janela();
    		void poll();
            inline void defCursor(const cursor c) {
                if(c == m_cursor) return;
                m_cursor = c;
            }
    		void swap();
	    	void viewport() const;
            std::string nome() const;
    		void nome(const char* novo_nome);
            janela() = default;
	};
    inline static janela* instanciaAtual { nullptr };
}
