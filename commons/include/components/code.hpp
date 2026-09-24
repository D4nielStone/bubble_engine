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
 * @file code.hpp
 * @brief Componente de código para entities ECS.
 *
 * @see code.cpp
 */

#pragma once
#include <string>
#include <lua.hpp>
#include <sol/sol.hpp>
#include <rapidjson/rapidjson.h>
#include "commons_namespace.hpp"
#include "component.hpp"

namespace COMMONS_NS {
	struct code : component {
		static constexpr mask mask = COMPONENTE_CODIGO;
		std::string file;

		std::string arquivoCompleto;
        sol::state estado_lua;
        sol::function f_setup, f_update;

		/**
		* @brief carrega script lua
		*/
		code(const std::string& file);
		/**
		* @brief limpa recursos
		*/
		~code();
		code() = default;
        bool analyze(const rapidjson::Value&) override;
        bool serialize(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const override;
		void setup();
		void update();
	};
}
