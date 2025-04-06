/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
#include "componente.hpp"
#include "transformacao.hpp"
#include <string>
#include "depuracao/debug.hpp"
#include <lua.hpp>

extern "C" {
#include <lua5.3/lua.h>
}

namespace bubble
{
	struct codigo : componente
	{
		static constexpr mascara mascara = COMPONENTE_CODIGO;
		std::string arquivo;
		std::string arquivoCompleto;
		lua_State* L;

		/**
		* @brief carrega script lua
		*/
		codigo(const std::string& arquivo);
		/**
		* @brief limpa recursos
		*/
		~codigo();
		codigo() = default;
        bool analizar(const rapidjson::Value&) override;
        bool serializar(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const override;
		void iniciar() const;
		void encerrar();
		void atualizar() const;
	};
}
