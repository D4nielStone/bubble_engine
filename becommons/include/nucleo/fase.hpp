/** @copyright Copyright (c) 2025 Daniel Oliveira */

/**
 * @file fase.hpp
 * @brief Gerencia sistemas numa fase
 */

#pragma once
#include "entidades/entidade.hpp"
#include "nucleo/registro.hpp"
#include "componentes/camera.hpp"
#include <string>
#include <memory>
#include <queue>
#include "sistema_de_fisica.hpp"
#include "sistema_de_codigo.hpp"
#include "sistema_de_renderizacao.hpp"
#include "sistema_de_interface.hpp"

/**
 * @class fase
 */

namespace bubble
{
	inline std::queue<std::function<void()>> file_de_tarefas;
	class fase : public std::enable_shared_from_this<fase>
	{
	public:
		fase(const char* diretorio);
		fase(const std::string& diretorio);
		fase();
		~fase();
		std::string nome() const;
		void pausar();
		void parar();
		void iniciar();
		void atualizar(double deltaTime);
		void definirCamera(const entidade &ent);
		void analizar(const char* diretorio);
		void carregar();
		void descarregar();
		std::shared_ptr<camera> obterCamera() const;
		registro* obterRegistro();
		sistema_fisica sfisica;
		sistema_renderizacao srender;
		sistema_interface sinterface;
		sistema_codigo scodigo;	
	private:
		bool inicializacao = true;
		const char* diretorio;
		registro reg;
	 	std::shared_ptr<camera> camera_atual{ nullptr };
		bool rodando = false;
		std::string _Mnome {""};
	};
}
