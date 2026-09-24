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
 * @file component.hpp
 * @brief Estrutura base para componetes
 */

#pragma once
#include <unordered_map>
#include <cstdint>
#include <type_traits>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include "commons_namespace.hpp"

namespace COMMONS_NS {
	// Operadores bit a bit para mask

	/**
	* @struct component
	* @brief base para as funcionalidades da engine
	*/
	struct component
	{
		//
		// @enum mask
		// @brief facilita a identificacao dos components pertencentes a uma entity
		//
		enum mask : uint32_t {
			COMPONENTE_NONE = 0,
			COMPONENTE_CAM = 1 << 0,
			COMPONENTE_RENDER = 1 << 1,
			COMPONENTE_PROPRIEDADES = 1 << 2,
			COMPONENTE_TRANSFORMACAO = 1 << 3,
			COMPONENTE_TEXTO = 1 << 4,
			COMPONENTE_CODIGO = 1 << 5,
			COMPONENTE_IMAGEM = 1 << 6,
			COMPONENTE_FISICA = 1 << 7,
			COMPONENTE_LUZ_PONTUAL = 1 << 8,
			COMPONENTE_LUZ_DIRECIONAL = 1 << 9,
			COMPONENTE_LUZ_HOLOFOTE = 1 << 10,
			COMPONENTE_TERRENO = 1 << 11
		};
		//  Mapeamento de nomes e mask
        inline static std::unordered_map<mask, std::string> mapa_nomes_componentes = {
            {mask::COMPONENTE_LUZ_DIRECIONAL            ,"Iluminacao"},
            {mask::COMPONENTE_LUZ_PONTUAL               ,"Iluminacao"},
            {mask::COMPONENTE_RENDER                    ,"Renderizador"},
            {mask::COMPONENTE_TRANSFORMACAO             ,"Transformacao"},
            {mask::COMPONENTE_CODIGO                    ,"Codigo"},
            {mask::COMPONENTE_CAM                       ,"Camera"},
            {mask::COMPONENTE_TERRENO                   ,"Terreno"}
        };
        // Id da entity ECS atual
        // Todo component deve ter um e ele é utilizado para encontrar o "eu" (entity do component de código) para a api lua.
		uint32_t my_object{ 0 };
		component() = default;
		virtual ~component() = default;
	    virtual bool analyze(const rapidjson::Value& obj) {return true;};
        virtual bool serialize(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const{return true;};
	};
	inline component::mask operator|(component::mask lhs, component::mask rhs) {
		using T = std::underlying_type_t<component::mask>;
		return static_cast<component::mask>(static_cast<T>(lhs) | static_cast<T>(rhs));
	}
	inline component::mask& operator|=(component::mask& lhs, component::mask rhs) {
		lhs = lhs | rhs;
		return lhs;
	}
	inline component::mask operator&(component::mask lhs, component::mask rhs) {
		using T = std::underlying_type_t<component::mask>;
		return static_cast<component::mask>(static_cast<T>(lhs) & static_cast<T>(rhs));
	}
	inline component::mask& operator&=(component::mask& lhs, component::mask rhs) {
		lhs = lhs & rhs;
		return lhs;
	}
	inline component::mask operator~(component::mask mask) {
		using T = std::underlying_type_t<component::mask>;
		return static_cast<component::mask>(~static_cast<T>(mask));
	}
}
