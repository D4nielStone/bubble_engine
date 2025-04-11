/// @file mat.hpp
/// @brief Funções de matemática referentes à api lua

#pragma once
#include <glm/glm.hpp>
#include <glm/detail/type_vec3.hpp>
#include "namespace.hpp"

namespace BECOMMONS_NS {
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
	inline static float distancia2(const vetor2<double>& a, const vetor2<double>& b)
	{
		float dx = b.x - a.x;
		float dy = b.y - a.y;

		return std::sqrt(dx * dx + dy * dy );
	}
} // namespace becommons
inline glm::vec3 operator*(const glm::vec3& lhs, const glm::vec3& rhs)
{
	return glm::vec3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
using namespace BECOMMONS_NS;
