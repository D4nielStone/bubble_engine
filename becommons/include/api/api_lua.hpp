/// @file api_lua.hpp
/// @brief Gerencia a configuração e registro da API do motor com Lua via LuaBridge.
/// 
/// Este arquivo é responsável por expor componentes internos do motor gráfico ao ambiente
/// de scripts Lua utilizando a biblioteca LuaBridge. Ele define a estrutura `entidade`, que
/// agrega múltiplos componentes possíveis de uma entidade de jogo (transformação, física, câmera,
/// texto, imagem, renderizador e luz direcional), e oferece funções auxiliares para registrar
/// utilitários, física, tempo e entradas no estado da máquina virtual Lua.
/// 
/// Ao incluir este cabeçalho e chamar as funções apropriadas de definição, o motor pode ser
/// facilmente estendido e controlado por meio de scripts Lua, facilitando a criação de lógicas
/// personalizadas no jogo em tempo de execução.
/// 
/// Componentes expostos:
/// - Transformação (posição, rotação, escala)
/// - Física (integração com Bullet Physics)
/// - Câmera (visão da cena)
/// - Texto e imagem (elementos de interface)
/// - Luz direcional e renderizador (sistema gráfico)
/// 
/// ### Exemplo de uso no script lua:
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.py
/// local Teste
///
/// -- Função padrão de inicialização
/// function iniciar()
///     Teste = entidade(1) -- Primeira entidade (possui transformação)
///     Teste:transformacao.posicao.x = 0 -- Altera o componente.
/// end
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// @author Daniel O. dos Santos
/// @copyright Copyright (c) 2025 Daniel Oliveira
/// @see api_lua.cpp

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
    namespace api {
    /// Estrutura para api lua
    /// @note É diferente da entidade do sistema ECS, aqui serve como uma ponte para a api lua
	struct entidade
	{
	    /// @name Componentes-propriedade da entidade
	    /// Conjunto de ponteiros para a entidade na api lua
	    /// @{
		transformacao* m_transformacao{ nullptr };
		fisica* m_fisica{ nullptr };
		camera* m_camera{ nullptr };
		texto* m_texto{ nullptr };
		imagem* m_imagem{ nullptr };
        luz_direcional* m_luzDir{nullptr};
        renderizador* m_renderizador{nullptr};
        /// @}
        /// Id baseado no sistema ECS para encontrar a entidade
		uint32_t id;
		/// Define o Lua State da api lua
		/// @see codigo.cpp
		static void definir(lua_State* L);
		void destruir() const;
		entidade(const uint32_t& id);
	};
    /// @name Definições pŕoprias
    /// @{
	  	void definirFisica(lua_State* L);
	  	void definirTempo(lua_State* L);
	  	void definirUtils(lua_State* L);
	  	void definirInputs(lua_State* L);
	/// @}
   } /// namespace api
} /// namespace becommons
