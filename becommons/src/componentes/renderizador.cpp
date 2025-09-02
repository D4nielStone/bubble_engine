/** \copyright
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
 * LIABILITY, WHETHER IN AN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * @file renderizador.cpp
 */

#include "componentes/renderizador.hpp"
#include "nucleo/engine.hpp"

using namespace becommons;

bool renderizador::serializar(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const {
    value.AddMember("modelo", rapidjson::Value(m_modelo->obterDiretorio().c_str(), allocator), allocator);
    return true;
};
bool renderizador::analizar(const rapidjson::Value& value) {
    if(m_modelo) delete m_modelo;
    if(value.HasMember("modelo") && value["modelo"].IsString()) {
        auto m_diretorio = std::string(value["modelo"].GetString());
        m_modelo = new modelo(m_diretorio);
    }
    else return false;
	return true;
};
		
renderizador::renderizador(const char* m_diretorio) {
    m_modelo = new modelo(m_diretorio);
    motor::obter().fila_opengl.push([&](){
            m_modelo->carregar();
    });
};
