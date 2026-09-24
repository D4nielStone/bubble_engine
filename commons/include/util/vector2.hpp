/** @copyright
MIT License
Copyright (c) 2025 Daniel Oliveira

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
 * @file vector2.hpp
 */

#pragma once
#include <cmath>
#include "commons_namespace.hpp"

namespace COMMONS_NS {
    template<class T>
    struct vector2
    {
        T x, y;

        // Constructor com par�metros
        vector2(T x, T y) : x(x), y(y) {}

        // Constructor padr�o
        vector2() : x(T{}), y(T{}) {}

        // Deve somar
        vector2 operator+(const vector2& other) const
        {
            return vector2{ x + other.x, y + other.y };
        };
        vector2 operator+(const float other) const
        {
            return vector2{ x + other, y + other };
        };
        vector2& operator+=(const vector2& other)
        {
            x += other.x;
            y += other.y;
            return *this;
        };
        vector2& operator+=(const float other)
        {
            x += other;
            y += other;
            return *this;
        };
        // Deve subtrair
        vector2 operator-(const vector2& other) const
        {
            return vector2{ x - other.x, y - other.y };
        };
        vector2 operator-(const float other) const
        {
            return vector2{ x - other, y - other};
        };
        vector2& operator-=(const vector2& other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        };
        vector2& operator-=(const float other)
        {
            x -= other;
            y -= other;
            return *this;
        };
        // Deve multiplicar
        vector2 operator*(const vector2& other)
        {
            return vector2{ x * other.x, y * other.y };
        };
        vector2 operator*(float other)
        {
            return vector2{ x * other, y * other };
        };
        vector2 operator*=(const vector2& other)
        {
            x *= other.x;
            y *= other.y;
            return *this;
        };
        vector2 operator*=(float other)
        {
            x *= other;
            y *= other;
            return *this;
        };
        // Deve dividir
        vector2 operator/(const vector2& other)
        {
            return vector2{ x / other.x, y / other.y};
        };
        vector2 operator/(float other)
        {
            return vector2{ x / other, y / other };
        };
        vector2 operator/=(const vector2& other)
        {
            x /= other.x;
            y /= other.y;
            return *this;
        };
        vector2 operator/=(float other)
        {
            x /= other;
            y /= other;
            return *this;
        };
        // Operador de igualdade (==)
        bool operator==(const vector2& other) const
        {
            return x == other.x && y == other.y;
        }
        // Operador de diferen�a (!=)
        bool operator!=(const vector2& other) const
        {
            return !(*this == other);
        }
        void normalize()
        {
            float mag = std::sqrt(x*x + y*y);
            if(mag < 0) return;
            x = x / mag;
            y = y / mag;
        }
        T size() const
        {
            return std::sqrt(x * x + y * y);
        }
    };
}

typedef COMMONS_NS::vector2<float> fvector_type2;
typedef COMMONS_NS::vector2<double> dvector_type2;
typedef COMMONS_NS::vector2<int> ivector_type2;
