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
#include "namespace.hpp"
#include "componentes/componente.hpp"

// Namespace bubble
namespace BECOMMONS_NS {
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

}

// Implementação de std::hash para entidade
namespace std {
	template <>
	struct hash<BECOMMONS_NS::entidade> {
		std::size_t operator()(const BECOMMONS_NS::entidade& entidade) const {
			return std::hash<uint32_t>()(entidade.id);
		}
	};
}
