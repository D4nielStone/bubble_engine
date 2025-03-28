/** @copyright Copyright (c) 2025 Daniel Oliveira */

/**
 * @file janela.hpp
 * @brief Inicia e gerencia janela glfw
 */

#pragma once
#include "tempo.hpp"
#include <GLFW/glfw3.h>
#include "util/vetor2.hpp"
#include "nucleo/sistema.hpp"
#include "nucleo/sistema_de_renderizacao.hpp"
#include "inputs/inputs.hpp"
#include <functional>
#include <memory>
#include <unordered_map>

namespace bubble
{
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
        public:
		tempo _Mtempo;
        std::string _Mnome{""};
		bubble::vetor2<double> tamanho;
		GLFWwindow* window;
		bubble::inputs inputs;
		janela(const char* nome, bubble::vetor2<double> bounds = bubble::vetor2<double>(600, 400), const char* icon_path = nullptr);
		~janela();
		void poll() const;
        inline void defCursor(const cursor c)
        {
            if(c == m_cursor) return;
            m_cursor = c;
            auto cursor_glfw = glfwCreateStandardCursor((int)c);
            if(cursor_glfw)
            glfwSetCursor(window, cursor_glfw);
        }
		void swap();
		void viewport() const;
        std::string nome() const;
		void nome(const char* novo_nome);
	};
	inline janela* instanciaJanela{ nullptr };
}
