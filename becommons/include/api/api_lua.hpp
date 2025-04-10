/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file api_lua.hpp
 * @brief Gerencia a configuração do luabridge para api lua.
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 *
 * @see api_lua.cpp
 */

#pragma once
#include <cstdint>
#include <bullet/btBulletDynamicsCommon.h>
#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include "namespace.hpp"
#include "componentes/luz_direcional.hpp"
#include "componentes/imagem.hpp"
#include "componentes/texto.hpp"
#include "componentes/camera.hpp"
#include "componentes/fisica.hpp"
#include "componentes/transformacao.hpp"
#include "componentes/renderizador.hpp"

namespace BECOMMONS_NS {
    namespace api{
	struct entidade
	{
		transformacao* m_transformacao{ nullptr };
		fisica* m_fisica{ nullptr };
		camera* m_camera{ nullptr };
		texto* m_texto{ nullptr };
		imagem* m_imagem{ nullptr };
        luz_direcional* m_luzDir{nullptr};
        renderizador* m_renderizador{nullptr};
		uint32_t id;
		static void definir(lua_State* L);
		void destruir() const;
		entidade(const uint32_t& id);
	};
	  	void definirFisica(lua_State* L);
	  	void definirTempo(lua_State* L);
	  	void definirUtils(lua_State* L);
	  	void definirInputs(lua_State* L);
   }
}
