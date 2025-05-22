/** @copyright 
 * MIT License
 * Copyright (c) 2025 Daniel Oliveira
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 */
/**
 * @file cor.hpp
 */

#pragma once
#include "becommons_namespace.hpp"
#include <algorithm>
// Definição da estrutura cor
namespace BECOMMONS_NS {
    struct cor
    {
        float r = 1.f, g = 1.f, b = 1.f, a = 1.f;
        cor(const float& f) : r(f), g(f), b(f), a(1.f) {}
        cor(const float& r, const float& g, const float& b, const float& a) : r(r), g(g), b(b), a(a) {};
        cor(const float& r, const float& g, const float& b) : r(r), g(g), b(b) {};
        cor() = default;
        // Operador de soma (+)
        cor operator+(const cor& other) const
        {
            return { r + other.r, g + other.g, b + other.b, std::min(1.f, a + other.a) };
        }
        // Operador de soma (+)
        cor operator+(const float other) const
        {
            return { r + other, g + other, b + other, std::min(1.f, a + other) };
        }
        // Operador de igualdade (==)
        bool operator==(const cor& other) const
        {
            return r == other.r && g == other.g && b == other.b && a == other.a;
        }
        // Operador de diferen�a (!=)
        bool operator!=(const cor& other) const
        {
            return !(*this == other);
        }
    };
}
