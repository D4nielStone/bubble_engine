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
 * @file componente.hpp
 * @brief Estrutura base para componetes
 */

#pragma once
#include <unordered_map>
#include <cstdint>
#include <type_traits>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include "becommons_namespace.hpp"

namespace BECOMMONS_NS {
	// Operadores bit a bit para mascara

	/**
	* @struct componente
	* @brief base para as funcionalidades da engine
	*/
	struct componente
	{
		//
		// @enum mascara
		// @brief facilita a identificacao dos componentes pertencentes a uma entidade
		//
		enum mascara : uint32_t {
			COMPONENTE_CAM =            1 << 9,
			COMPONENTE_FISICA =         1 << 8,
			COMPONENTE_RENDER =         1 << 7,
			COMPONENTE_CODIGO =         1 << 6,
			COMPONENTE_LUZ_PONTUAL =    1 << 5,
			COMPONENTE_LUZ_HOLOFOTE =   1 << 4,
			COMPONENTE_LUZ_DIRECIONAL = 1 << 3,
			COMPONENTE_TERRENO =        1 << 2,
			COMPONENTE_TRANSFORMACAO =  1 << 1,
			COMPONENTE_NONE =                0
		};
		//  Mapeamento de nomes e mascara
        inline static std::unordered_map<mascara, std::string> mapa_nomes_componentes = {
            {mascara::COMPONENTE_LUZ_DIRECIONAL            ,"Iluminacao"},
            {mascara::COMPONENTE_LUZ_PONTUAL               ,"Iluminacao"},
            {mascara::COMPONENTE_FISICA                    ,"Fisica"},
            {mascara::COMPONENTE_RENDER                    ,"Renderizador"},
            {mascara::COMPONENTE_TRANSFORMACAO             ,"Transformacao"},
            {mascara::COMPONENTE_CODIGO                    ,"Codigo"},
            {mascara::COMPONENTE_CAM                       ,"Camera"},
            {mascara::COMPONENTE_TERRENO                   ,"Terreno"}
        };
        // Id da entidade ECS atual
        // Todo componente deve ter um e ele é utilizado para encontrar o "eu" (entidade do componente de código) para a api lua.
		uint32_t meu_objeto{ 0 };
		componente() = default;
		virtual ~componente() = default;
	    virtual bool analizar(const rapidjson::Value& obj) {return true;};
        virtual bool serializar(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const{return true;};
	};
	inline componente::mascara operator|(componente::mascara lhs, componente::mascara rhs) {
		using T = std::underlying_type_t<componente::mascara>;
		return static_cast<componente::mascara>(static_cast<T>(lhs) | static_cast<T>(rhs));
	}
	inline componente::mascara& operator|=(componente::mascara& lhs, componente::mascara rhs) {
		lhs = lhs | rhs;
		return lhs;
	}
	inline componente::mascara operator&(componente::mascara lhs, componente::mascara rhs) {
		using T = std::underlying_type_t<componente::mascara>;
		return static_cast<componente::mascara>(static_cast<T>(lhs) & static_cast<T>(rhs));
	}
	inline componente::mascara& operator&=(componente::mascara& lhs, componente::mascara rhs) {
		lhs = lhs & rhs;
		return lhs;
	}
	inline componente::mascara operator~(componente::mascara mask) {
		using T = std::underlying_type_t<componente::mascara>;
		return static_cast<componente::mascara>(~static_cast<T>(mask));
	}
}
