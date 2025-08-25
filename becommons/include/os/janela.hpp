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
	class ijanela{
    public:
        enum class cursor : uint32_t {
            mao = 0x00036004,
            i   = 0x00036002,
            seta= 0x00036001,
            re_h= 0x00036005,
            re_v= 0x00036006
        };
        ijanela() = default;
        virtual ~ijanela() = default;

        // Processa eventos (no real: glfwPollEvents)
        virtual void poll() {};

        // Troca buffers / apresenta frame
        virtual void swap() {};

        // Retorna true se a janela deve fechar
        virtual bool deveFechar() const {return false;};

        // Retorna o tamanho da janela
        virtual ivet2 obterTamanho() const {return {100,100};};

        // Posiciona o cursor da janela
        virtual void posicionarCursor(double x, double y) {};

        // Define o estado do cursor
        virtual void definirCursor(const cursor c) {};

        // Efetua o viewprt gráfico com as dimenções da janela
		virtual void viewport() const {};
    };
    /**
	* @class janela
	* @brief facilita o uso da janela glfw
	* @brief cuida dos callbacks, parametros iniciais etc.
	*/
	class janela : public ijanela {
    public:
    	janela(const char* nome, fvet2 bounds = fvet2(600, 400), const char* icon_path = nullptr);
    	janela(const char* nome, const bool f, fvet2 bounds = fvet2(600, 400), const char* icon_path = nullptr);
    	~janela();

    	void poll                               () override;
    	void swap                               () override;
        bool deveFechar                         () const override;
		void viewport                           () const override;
        ivet2 obterTamanho                      () const override;
        void posicionarCursor                   (double x, double y) override;
        void definirCursor                      (const cursor c) override;

        std::string nome                        () const;
    	void nome                               (const char* novo_nome);
        
    	GLFWwindow* window;
    private:
    	ivet2 tamanho;
        std::string m_nome{""};
        cursor m_cursor = cursor::seta;
	    cursor m_cursor_antigo = cursor::seta;
	};
}
