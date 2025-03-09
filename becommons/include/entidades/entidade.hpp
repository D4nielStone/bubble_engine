/** @copyright Copyright (c) 2025 Daniel Oliveira */

/**
 * @file entidade.hpp
 * @brief Inclusoes e defini��es relacionadas � entidade
 */

#pragma once
#include <memory>
#include "componentes/componente.hpp"

namespace bubble
{
	/**
	 * @struct entidade
	 * @brief todo elemento do faserio
	 */
	struct entidade {
		uint32_t id;
		componente::mascara mascara = componente::COMPONENTE_NONE;

		bool operator==(const entidade& other) const {
			return id == other.id;
		}
	};

}	///< namespace bubble

// Implementa��o de std::hash para entidade
namespace std {
	template <>
	struct hash<bubble::entidade> {
		std::size_t operator()(const bubble::entidade& entidade) const {
			return std::hash<uint32_t>()(entidade.id);
		}
	};
}