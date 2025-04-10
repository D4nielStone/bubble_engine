/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file janela.hpp
 * @brief Inicia e gerencia janela glfw
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 *
 * @see janela.cpp
 */

#pragma once
#include <functional>
#include <memory>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "namespace.hpp"
#include "tempo.hpp"
#include "util/vetor2.hpp"

namespace BECOMMONS_NS
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
	    cursor m_cursor_antigo = cursor::seta;
        public:
		tempo m_tempo;
        std::string m_nome{""};
		vetor2<double> tamanho;
		GLFWwindow* window;
		inputs m_inputs;
		janela(const char* nome, vetor2<double> bounds = vetor2<double>(600, 400), const char* icon_path = nullptr);
		janela(const char* nome, const bool f, vetor2<double> bounds = vetor2<double>(600, 400), const char* icon_path = nullptr);
		~janela();
		void poll();
        inline void defCursor(const cursor c)
        {
            if(c == m_cursor) return;
            m_cursor = c;
        }
		void swap();
		void viewport() const;
        std::string nome() const;
		void nome(const char* novo_nome);
        private:
            janela() = default;
	};
	inline janela* instanciaJanela{ nullptr };
}
