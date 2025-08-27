/**
 *  @file mat.hpp
 *  Funções de matemática referentes à api lua
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
#include "util/vetor2.hpp"
#include "util/vetor3.hpp"
#include "util/vetor4.hpp"

/** namespace becommons */
namespace becommons {
    static inline float lerp(const float& start, const float& end, const float& alpha) {
        return start + alpha * (end - start);
    }
    static inline fvet2 lerp(const fvet2& start, const fvet2& end, const float& alpha) {
        return fvet2(
                start.x + alpha * (end.x - start.x),
                start.y + alpha * (end.y - start.y)
                );
    }
    static inline fvet3 lerp(const fvet3& start, const fvet3& end, const float& alpha) {
        return fvet3(
                start.x + alpha * (end.x - start.x),
                start.y + alpha * (end.y - start.y),
                start.z + alpha * (end.z - start.z)
                );
    }
    static inline fvet4 lerp(const fvet4& start, const fvet4& end, const float& alpha) {
        return fvet4(
                start.x + alpha * (end.x - start.x),
                start.y + alpha * (end.y - start.y),
                start.z + alpha * (end.z - start.z),
                start.w + alpha * (end.w - start.w)
                );
    }
	static inline float dist(const float& a, const float& b) {
		return std::abs(a - b);
	}
	static inline float dist(const fvet2& a, const fvet2& b) {
	    float dx = b.x - a.x;
        float dy = b.y - a.y;
        return std::sqrt(dx * dx + dy * dy);
    }
	static inline float dist(const fvet3& a, const fvet3& b) {
	    float dx = b.x - a.x;
        float dy = b.y - a.y;
        float dz = b.z - a.z;
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }
}
// @see api_lua.cpp
