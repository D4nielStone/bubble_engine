/** @copyright 
MIT LicenseCopyright (c) 2025 Daniel Oliveira

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
*/
/**
 * @file vetor2.hpp
 */

#pragma once
#include <cmath>
#include "namespace.hpp"

namespace BECOMMONS_NS {
    template<class T>
    struct vetor2
    {
        T x, y;

        // Construtor com par�metros
        vetor2(T x, T y) : x(x), y(y) {}

        // Construtor padr�o
        vetor2() : x(T{}), y(T{}) {}

        // Deve somar
        vetor2 operator+(const vetor2& other) const
        {
            return vetor2{ x + other.x, y + other.y };
        };
        vetor2 operator+(const float other) const
        {
            return vetor2{ x + other, y + other };
        };
        vetor2& operator+=(const vetor2& other)
        {
            x += other.x;
            y += other.y;
            return *this;
        };
        vetor2& operator+=(const float other)
        {
            x += other;
            y += other;
            return *this;
        };
        // Deve subtrair
        vetor2 operator-(const vetor2& other) const
        {
            return vetor2{ x - other.x, y - other.y };
        };
        vetor2 operator-(const float other) const
        {
            return vetor2{ x - other, y - other};
        };
        vetor2& operator-=(const vetor2& other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        };
        vetor2& operator-=(const float other)
        {
            x -= other;
            y -= other;
            return *this;
        };
        // Deve multiplicar
        vetor2 operator*(const vetor2& other)
        {
            return vetor2{ x * other.x, y * other.y };
        };
        vetor2 operator*(float other)
        {
            return vetor2{ x * other, y * other };
        };
        vetor2 operator*=(const vetor2& other)
        {
            x *= other.x;
            y *= other.y;
            return *this;
        };
        vetor2 operator*=(float other)
        {
            x *= other;
            y *= other;
            return *this;
        };
        // Deve dividir
        vetor2 operator/(const vetor2& other)
        {
            return vetor2{ x / other.x, y / other.y};
        };
        vetor2 operator/(float other)
        {
            return vetor2{ x / other, y / other };
        };
        vetor2 operator/=(const vetor2& other)
        {
            x /= other.x;
            y /= other.y;
            return *this;
        };
        vetor2 operator/=(float other)
        {
            x /= other;
            y /= other;
            return *this;
        };
        // Operador de igualdade (==)
        bool operator==(const vetor2& other) const
        {
            return x == other.x && y == other.y;
        }
        // Operador de diferen�a (!=)
        bool operator!=(const vetor2& other) const
        {
            return !(*this == other);
        }
        void normalizar()
        {
            float mag = std::sqrt(x*x + y*y);
            if(mag < 0) return;
            x = x / mag;
            y = y / mag;
        }
        T tamanho() const
        {
            return std::sqrt(x * x + y * y);
        }
    };
}

typedef BECOMMONS_NS::vetor2<float> fvet2;
typedef dvet2 dvet2;
typedef BECOMMONS_NS::vetor2<int> ivet2;
