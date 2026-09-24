#pragma once
#include <memory>
#include "commons_namespace.hpp"
#include "components/component.hpp"

namespace COMMONS_NS {
	/**
	 * @struct entity
	 * @brief todo elemento do faserio
	 */
	struct entity {
	    // Id da entity no system ECS
		uint32_t id;
		// Flags de components
        component::mask mask = component::COMPONENTE_NONE;

        // Operador de igualdade
		bool operator==(const entity& other) const {
			return id == other.id;
		}
	};
}   // Namespace commons

// Implementação de std::hash para entity
namespace std {
	template <>
	struct hash<COMMONS_NS::entity> {
		std::size_t operator()(const COMMONS_NS::entity& entity) const {
			return std::hash<uint32_t>()(entity.id);
		}
	};
}
