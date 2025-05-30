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
 * @file codigo.cpp
 */

#include "api/api_lua.hpp"
#include "componentes/codigo.hpp"
#include "nucleo/fase.hpp"
#include "nucleo/projeto.hpp"
#include "api/mat.hpp"
#include "inputs/inputs.hpp"
#include "os/janela.hpp"
#include "depuracao/debug.hpp"
#include "os/sistema.hpp"
#include <cmath>

using namespace BECOMMONS_NS;

codigo::codigo(const std::string& arquivo) : arquivo(arquivo) {
    arquivoCompleto = projeto_atual->diretorioDoProjeto + arquivo; 

	estado_lua.open_libraries(sol::lib::base);

/*	api::definirFisica(estado_lua);
	api::definirTempo(estado_lua);
	api::definirUtils(estado_lua);
	api::definirInputs(estado_lua);
	api::entidade::definir(estado_lua);*/
	
	estado_lua.script_file(arquivoCompleto);
    // Obtém as funções globais
    f_iniciar = estado_lua["iniciar"];
    f_atualizar = estado_lua["atualizar"];
}

void codigo::iniciar() {
	estado_lua.script_file(arquivoCompleto);
    // Obtém as funções globais
    f_iniciar = estado_lua["iniciar"];
    f_atualizar = estado_lua["atualizar"];

   if(f_iniciar.valid()) f_iniciar();
}

bool codigo::analizar(const rapidjson::Value& value) {
    if(!value.HasMember("diretorio")) return false;
    arquivo = value["diretorio"].GetString();
    arquivoCompleto = projeto_atual->diretorioDoProjeto + arquivo; 

	estado_lua.open_libraries(sol::lib::base);

	api::definirClasses(estado_lua);
	api::definirNamespaces(estado_lua);
	
    return true;
};

bool codigo::serializar(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const {
    value.AddMember("diretorio", rapidjson::Value(arquivo.c_str(), allocator), allocator);
    return true;
}

void codigo::atualizar() {
   if(f_atualizar.valid()) f_atualizar();
}
codigo::~codigo() {
    depuracao::emitir(debug, "codigo", "descarregando");
}
