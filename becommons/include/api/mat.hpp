/// @file mat.hpp
/// Funções de matemática referentes à api lua
/// Essas fuções serão passadas pelo luabridge para o Lua_State.
///
/// @copyright 
/// MIT License
/// Copyright (c) 2025 Daniel Oliveira
/// 
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
/// 
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
/// 
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE. 
/// 
/// @see api_lua.cpp

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
	inline static float distancia2(const dvet2& a, const dvet2& b)
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
