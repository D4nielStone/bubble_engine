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
 * @file entidade.hpp
 * @brief Inclusoes e definições relacionadas à entidade/
 */

#pragma once
#include <memory>
#include "becommons_namespace.hpp"
#include "componentes/componente.hpp"

namespace BECOMMONS_NS {
	/**
	 * @struct entidade
	 * @brief todo elemento do faserio
	 */
	struct entidade {
	    // Id da entidade no sistema ECS
		uint32_t id;
		// Flags de componentes
        componente::mascara mascara = componente::COMPONENTE_NONE;
    
        // Operador de igualdade
		bool operator==(const entidade& other) const {
			return id == other.id;
		}
	};
}   // Namespace becommons

// Implementação de std::hash para entidade
namespace std {
	template <>
	struct hash<BECOMMONS_NS::entidade> {
		std::size_t operator()(const BECOMMONS_NS::entidade& entidade) const {
			return std::hash<uint32_t>()(entidade.id);
		}
	};
}
