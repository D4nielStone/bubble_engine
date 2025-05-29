/** @copyright 
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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 * @file api_lua.cpp
 */

#include "api/api_lua.hpp"
#include "nucleo/fase.hpp"
#include "nucleo/projeto.hpp"
#include "componentes/renderizador.hpp"
#include "componentes/transformacao.hpp"
#include "componentes/terreno.hpp"
#include "componentes/texto.hpp"
#include "componentes/codigo.hpp"
#include "nucleo/sistema_de_fisica.hpp"
#include "os/janela.hpp"
#include "inputs/inputs.hpp"
#include "util/malha.hpp"
#include "util/vertice.hpp"
#include "api/mat.hpp"

using namespace BECOMMONS_NS;
/**  Define as classes da api */
void api::definirClasses(sol::state& lua) {
}
/**  Define os namespaces da api como math e inputs */
void api::definirNamespaces(sol::state& lua) {
}
