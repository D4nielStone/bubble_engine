/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file mat.hpp
 * @brief Gerencia funçoes matemáticas
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */


#pragma once
#include <glm/glm.hpp>
#include <glm/detail/type_vec3.hpp>
#include "util/vetor2.hpp"

namespace bubble
{
	inline static float distancia3(const glm::vec3& a, const glm::vec3& b)
	{
		float dx = b.x - a.x;
		float dy = b.y - a.y;
		float dz = b.z - a.z;

		return std::sqrt(dx * dx + dy * dy + dz * dz);
	}
	inline static glm::vec3 lerpV3(const glm::vec3& a, const glm::vec3& b, const float t)
	{
		return a + t * (b - a);
	}
	inline static float distancia2(const bubble::vetor2<double>& a, const bubble::vetor2<double>& b)
	{
		float dx = b.x - a.x;
		float dy = b.y - a.y;

		return std::sqrt(dx * dx + dy * dy );
	}
}
inline glm::vec3 operator*(const glm::vec3& lhs, const glm::vec3& rhs)
{
	return glm::vec3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
