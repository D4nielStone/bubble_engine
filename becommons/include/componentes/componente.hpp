
/** @copyright Copyright (c) 2025 Daniel Oliveira */

/**
 * @file componente.hpp
 * @brief Estrutura base para componetes
 */
#pragma once
#include <cstdint>
#include <type_traits>

namespace bubble
{
	// Operadores bit a bit para mascara

	/**
	* @struct componente
	* @brief base para as funcionalidades da engine
	*/
	struct componente
	{
		/**
		* @enum mascara
		* @brief facilita a identificacao dos componentes pertencentes a uma entidade
		*/
		enum mascara : uint32_t
		{
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
		uint32_t meu_objeto{ 0 };
		componente() = default;
		virtual ~componente() = default;
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