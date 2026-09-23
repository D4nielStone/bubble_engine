#pragma once
#include <functional>
#include <memory>
#include <unordered_map>
#include "glad.h"
#include <GLFW/glfw3.h>
#include "commons_namespace.hpp"
#include "tempo.hpp"
#include "util/vetor2.hpp"
#include "inputs/inputs.hpp"

namespace COMMONS_NS {
	/**
	* @struct window
	* @brief facilita o uso da window glfw
	* @brief cuida dos callbacks, parametros iniciais etc.
	*/
	struct window {
        enum class cursor : int {
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
            static void posicionarCursor                   (double x, double y);
            static ivet2 obterTamanhoWindow                ();
            static bool temInstancia();
            static window& obterInstancia();
            static window& newInstance(const char* nome, fvet2 bounds = fvet2(600, 400), const char* icon_path = nullptr);
    		static window& newInstance(const char* nome, const bool f, fvet2 bounds = fvet2(600, 400), const char* icon_path = nullptr);

    		tempo m_tempo;
            std::string m_nome{""};
    		ivet2 tamanho;
    		GLFWwindow* m_window;
    		inputs m_inputs;
    		window(const char* nome, fvet2 bounds = fvet2(600, 400), const char* icon_path = nullptr);
    		window(const char* nome, const bool f, fvet2 bounds = fvet2(600, 400), const char* icon_path = nullptr);
    		~window();
    		void poll();
            void loop();
            inline void defCursor(const cursor c) {
                if(c == m_cursor) return;
                m_cursor = c;
            }
    		void swap();
	    	void viewport() const;
            std::string nome() const;
    		void nome(const char* novo_nome);
            window() = default;
	};
    inline static window* instanciaAtual { nullptr };
}
