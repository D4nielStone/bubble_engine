/**
 *  \file api_lua.hpp
 *  Gerencia a configuração e registro da API do motor com Lua via LuaBridge.
 *  
 *  Este arquivo é responsável por expor componentes internos do motor gráfico ao ambiente
 *  de scripts Lua utilizando a biblioteca LuaBridge. Ele define a estrutura `entidade`, que
 *  agrega múltiplos componentes possíveis de uma entidade de jogo (transformação, física, câmera,
 *  texto, imagem, renderizador e luz direcional), e oferece funções auxiliares para registrar
 *  utilitários, física, tempo e entradas no estado da máquina virtual Lua.
 *  
 *  Ao incluir este cabeçalho e chamar as funções apropriadas de definição, o motor pode ser
 *  facilmente estendido e controlado por meio de scripts Lua, facilitando a criação de lógicas
 *  personalizadas no jogo em tempo de execução.
 *  
 *  Componentes expostos:
 *  - Transformação (posição, rotação, escala)
 *  - Física (integração com Bullet Physics)
 *  - Câmera (visão da cena)
 *  - Texto e imagem (elementos de interface)
 *  - Luz direcional e renderizador (sistema gráfico)
 *  
 *  ### Exemplo de uso no script lua:
 *  \code {lua}
 *  local Teste
 * 
 *  -- Função padrão de inicialização
 *  function iniciar()
 *      Teste = entidade(1) -- Primeira entidade (possui transformação)
 *      Teste:transformacao.posicao.x = 0 -- Altera o componente.
 *  end
 *  \endcode
 *  
 *  \copyright 
 *  MIT License
 *  Copyright (c) 2025 Daniel Oliveira
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE. 
 *  
 *  \see api_lua.cpp
 */
#pragma once
#include <cstdint>
#include <bullet/btBulletDynamicsCommon.h>
#include <sol/sol.hpp>
#include "componentes/fisica.hpp"
#include "componentes/renderizador.hpp"
#include "componentes/transformacao.hpp"
#include "componentes/camera.hpp"
#include "becommons_namespace.hpp"

namespace BECOMMONS_NS {
    namespace api {
        // \struct entidade
        // \brief Estrutura para api lua
        // \note É diferente da entidade do sistema ECS, aqui serve como uma ponte para a api lua
    	struct entidade {
    	    uint32_t id;
    		void destruir() const;
	    	entidade(const uint32_t& id);
		   	// \name Componentes-propriedade da entidade
		   	// Conjunto de ponteiros para a entidade na api lua
		   	// \{
		   	transformacao* m_transformacao{ nullptr };
		   	fisica* m_fisica{ nullptr };
		   	camera* m_camera{ nullptr };
           	renderizador* m_renderizador{nullptr};
            // \}
        };
	     /**  Define as classes da api */
	  	void definirClasses(sol::state&);    
         /**  Define os namespaces da api como math e inputs */
	  	void definirNamespaces(sol::state&);    
   } // < namespace api 
} // < namespace becommons
