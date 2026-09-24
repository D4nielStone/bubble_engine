#pragma once
#include <functional>
#include <memory>
#include <unordered_map>
#include "glad.h"
#include <GLFW/glfw3.h>
#include "commons_namespace.hpp"
#include "time.hpp"
#include "util/vector2.hpp"
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
            static ivector_type2 get_window_size                ();
            static bool hasInstance();
            static window& get_instance();
            static window& newInstance(const char* nome, fvector_type2 bounds = fvector_type2(600, 400), const char* icon_path = nullptr);
    		static window& newInstance(const char* nome, const bool f, fvector_type2 bounds = fvector_type2(600, 400), const char* icon_path = nullptr);

    		time m_time;
            std::string m_nome{""};
    		ivector_type2 size;
    		GLFWwindow* m_window;
    		inputs m_inputs;
    		window(const char* nome, fvector_type2 bounds = fvector_type2(600, 400), const char* icon_path = nullptr);
    		window(const char* nome, const bool f, fvector_type2 bounds = fvector_type2(600, 400), const char* icon_path = nullptr);
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
    inline static window* instanceAtual { nullptr };
}
