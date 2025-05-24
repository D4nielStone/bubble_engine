/** @copyright 
MIT License
Copyright (c) 2025 Daniel Oliveira

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
*/
/**
 * @file renderizador.hpp
 */

#pragma once
#include <rapidjson/rapidjson.h>
#include "becommons_namespace.hpp"
#include "nucleo/projeto.hpp"
#include "util/malha.hpp"
#include "arquivadores/modelo.hpp"
#include "componente.hpp"

namespace BECOMMONS_NS {
	class renderizador : public componente {
    public:
		modelo* m_modelo;
        std::string m_diretorio;
		static constexpr mascara mascara = COMPONENTE_RENDER;
		renderizador(modelo* malha) : m_modelo(malha) {
		}
        bool analizar(const rapidjson::Value& value) override
        {
            if(value.HasMember("modelo") && value["modelo"].IsString())
            {
                m_diretorio = std::string(value["modelo"].GetString());
				std::string path = projeto_atual->diretorioDoProjeto + m_diretorio;
                if(m_modelo) delete m_modelo;
                m_modelo = new modelo(path.c_str());
            }
            else return false;
			return true;
        };
        bool serializar(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const override
        {
            value.AddMember("modelo", rapidjson::Value(m_diretorio.c_str(), allocator), allocator);
            return true;
        };
        
		renderizador(const char* m_diretorio ) : m_modelo(new modelo(m_diretorio)) {};
		~renderizador()
		{
			for(auto& malha : m_modelo->malhas)
			{
				malha.descarregar();
			}
			delete m_modelo;
		}
        renderizador() = default;
	};
}
