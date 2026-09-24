/**
 *  @file api_lua.hpp
 *  Gerencia a configuração e ecs da API do motor com Lua via LuaBridge.
 *
 *  Este file é responsável por expor components internos do motor gráfico ao ambient
 *  de scripts Lua utilizando a biblioteca LuaBridge. Ele define a estrutura `entity`, que
 *  agrega múltiplos components possíveis de uma entity de jogo (transformação, física, câmera,
 *  text, image, renderer e luz direcional), e oferece funções auxiliares para registrar
 *  utilitários, física, time e entradas no estado da máquina virtual Lua.
 *
 *  Ao incluir este cabeçalho e chamar as funções apropriadas de definição, o motor pode ser
 *  facilmente estendido e controlado por meio de scripts Lua, facilitando a criação de lógicas
 *  personalizadas no jogo em time de execução.
 *
 *  Componentes expostos:
 *  - Transformação (posição, rotação, scale)
 *  - Física (integração com Bullet Physics)
 *  - Câmera (visão da cena)
 *  - Texto e image (elements de interface)
 *  - Luz direcional e renderer (system gráfico)
 *
 *  ### Exemplo de uso no script lua:
 *  \code {lua}
 *  local Teste
 *
 *  -- Função default de inicialização
 *  function setup()
 *      Teste = entity(1) -- Primeira entity (possui transformação)
 *      Teste:transform.position.x = 0 -- Altera o component.
 *  end
 *  \endcode
 *
 *  @copyright
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
 *  @see api_lua.cpp
 */
#pragma once
#include <cstdint>
#include <bullet/btBulletDynamicsCommon.h>
#include <sol/sol.hpp>
#include "commons_namespace.hpp"

namespace COMMONS_NS {
    namespace api {
	     /**  Define as classes da api */
	  	void setClasses(sol::state&);
         /**  Define os namespaces da api como math e inputs */
	  	void setNamespaces(sol::state&);
   } /// < namespace api
} /// < namespace commons
