/** \copyright
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
 * LIABILITY, WHETHER IN AN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * @see fase.cpp
 */


#pragma once
#include <string>
#include <memory>
#include <rapidjson/document.h>
#include "registro.hpp"
#include "componentes/luz_direcional.hpp"

/**
 * @struct fase
 */

namespace becommons {
	struct fase : public std::enable_shared_from_this<fase> {
		bool rodando{false};
        std::shared_ptr<luz_direcional> luz_global = nullptr;
	    /// @brief Construtores
		fase(const char* diretorio);
		fase(const std::string& diretorio);
		fase();
		~fase();

		/// @brief obtém nome
		/// @return string do nome da fase
		std::string nome() const;
        void nome(const std::string&);

        /// @brief deifni estado de run
		void pausar();
		void parar();
		void iniciar();


		void carregar(std::function<void(float)> onProgress = nullptr);
		void salvar();
		void descarregar();
		registro* obterRegistro();
	private:
	    void analizarEntidades(const rapidjson::Document&);
	    bool carregada{false};
		void analizar(const std::string& diretorio);
		void serializar(const std::string& diretorio);
        std::string diretorio;
		registro reg;
		std::string m_nome {""};
	};
}
