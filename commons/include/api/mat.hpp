/**
 *  @file mat.hpp
 *  Funções de mahasática referentes à api lua
 *  Essas fuções serão passadas pelo luabridge para o Lua_State.
 *
 *  @copyright
 *  MIT License
 *  Copyright (c) 2025 Daniel Oliveira
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */
#pragma once
#include "commons_namespace.hpp"
#include "util/vector2.hpp"
#include "util/vector3.hpp"
#include "util/vector4.hpp"

/** namespace commons */
namespace COMMONS_NS {
    static inline float lerp(const float& start, const float& end, const float& alpha) {
        return start + alpha * (end - start);
    }
    static inline fvector_type2 lerp(const fvector_type2& start, const fvector_type2& end, const float& alpha) {
        return fvector_type2(
                start.x + alpha * (end.x - start.x),
                start.y + alpha * (end.y - start.y)
                );
    }
    static inline fvector_type3 lerp(const fvector_type3& start, const fvector_type3& end, const float& alpha) {
        return fvector_type3(
                start.x + alpha * (end.x - start.x),
                start.y + alpha * (end.y - start.y),
                start.z + alpha * (end.z - start.z)
                );
    }
	static inline float dist(const float& a, const float& b) {
		return std::abs(a - b);
	}
}
// @see api_lua.cpp
