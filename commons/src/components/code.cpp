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
 * @file code.cpp

#include "api/api_lua.hpp"
#include "components/code.hpp"
#include "api/mat.hpp"
#include "inputs/inputs.hpp"
#include "os/window.hpp"
#include "debugging/debug.hpp"
#include "os/system.hpp"
#include <cmath>

using namespace COMMONS_NS;

code::code(const std::string& file) : file(file) {
    arquivoCompleto = current_project->directoryDoProjeto + file;

	estado_lua.open_libraries(sol::lib::base);

	api::setClasses(estado_lua);
	api::setNamespaces(estado_lua);
}

void code::setup() {
    estado_lua["bubble.meuID"] = my_object;
	estado_lua.script_file(arquivoCompleto);
    // Obtém as funções globais
    f_setup = estado_lua["setup"];
    f_update = estado_lua["update"];

   if(f_setup.valid()) f_setup();
}

bool code::analyze(const rapidjson::Value& value) {
    if(!value.HasMember("directory")) return false;
    file = value["directory"].GetString();
    arquivoCompleto = current_project->directoryDoProjeto + file;

	estado_lua.open_libraries(sol::lib::base);

	api::setClasses(estado_lua);
	api::setNamespaces(estado_lua);

    return true;
};

bool code::serialize(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const {
    value.AddMember("directory", rapidjson::Value(file.c_str(), allocator), allocator);
    return true;
}

void code::update() {
   if(f_update.valid()) f_update();
}
code::~code() {
    debugging::emit(debug, "code", "descarregando");
}
*/