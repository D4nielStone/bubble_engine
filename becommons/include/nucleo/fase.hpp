/** @copyright Copyright (c) 2025 Daniel Oliveira */

/**
 * @file fase.hpp
 * @brief Gerencia sistemas numa fase
 */

#pragma once
#include "nucleo/registro.hpp"
#include <string>
#include <memory>

/**
 * @struct fase
 */

namespace bubble
{
	struct fase : public std::enable_shared_from_this<fase>
	{
		bool rodando{false};
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


		void carregar();
		void descarregar();
		registro* obterRegistro();
	private:
	    bool carregada{false};
		void analizar(const std::string& diretorio);
        std::string diretorio;
		registro reg;
		std::string _Mnome {""};
	};
}
