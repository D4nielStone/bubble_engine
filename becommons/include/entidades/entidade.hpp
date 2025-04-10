/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file entidade.hpp
 * @brief Inclusoes e defini��es relacionadas � entidade
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */


#pragma once
#include <memory>
#include "becommons.hpp"

namespace BECOMMONS_NS
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

}	///< namespace BECOMMONS_NS

// Implementação de std::hash para entidade
namespace std {
	template <>
	struct hash<entidade> {
		std::size_t operator()(const entidade& entidade) const {
			return std::hash<uint32_t>()(entidade.id);
		}
	};
}
