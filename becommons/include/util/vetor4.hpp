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
 * @file vetor4.hpp
 */

#pragma once
#include <cmath>
#include "namespace.hpp"
/* Definição da estrutura vetor4 */

namespace BECOMMONS_NS {
    template<class T>
    struct vetor4
    {
        T x, y, z, w;

        vetor4(T x, T y, T z, T w) : x(x), y(y), w(w), z(z) {}
        vetor4() = default;
        // Deve somar
        vetor4 operator+(const vetor4& other) const
        {
            return vetor4{ x + other.x, y + other.y, z + other.z, w + other.w };
        };
        vetor4 operator+(const float other) const
        {
            return vetor4{ x + other, y + other, z + other, w + other };
        };
        vetor4& operator+=(const vetor4& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;
            return *this;
        };
        vetor4& operator+=(const float other)
        {
            x += other;
            y += other;
            z += other;
            w += other;
            return *this;
        };
        // Deve subtrair
        vetor4 operator-(const vetor4& other) const
        {
            return vetor4{ x - other.x, y - other.y, z - other.z, w - other.w };
        };
        vetor4 operator-(const float other) const
        {
            return vetor4{ x - other, y - other, z - other, w - other };
        };
        vetor4& operator-=(const vetor4& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            w -= other.w;
            return *this;
        };
        vetor4& operator-=(const float other)
        {
            x -= other;
            y -= other;
            z -= other;
            w -= other;
            return *this;
        };
        // Deve multiplicar
        vetor4 operator*(const vetor4& other)
        {
            return vetor4{ x * other.x, y * other.y, z * other.z, w * other.w };
        };
        vetor4 operator*(float other)
        {
            return vetor4{ x * other, y * other, z * other, w * other };
        };
        vetor4 operator*=(const vetor4& other)
        {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            w *= other.w;
            return *this;
        };
        vetor4 operator*=(float other)
        {
            x *= other;
            y *= other;
            z *= other;
            w *= other;
            return *this;
        };
        // Deve dividir
        vetor4 operator/(const vetor4& other)
        {
            return vetor4{ x / other.x, y / other.y, z / other.z, w / other.w };
        };
        vetor4 operator/(float other)
        {
            return vetor4{ x / other, y / other, z / other, w / other };
        };
        vetor4 operator/=(const vetor4& other)
        {
            x /= other.x;
            y /= other.y;
            z /= other.z;
            w /= other.w;
            return *this;
        };
        vetor4 operator/=(float other)
        {
            x /= other;
            y /= other;
            z /= other;
            w /= other;
            return *this;
        };
        // Operador de igualdade (==)
        bool operator==(const vetor4& other) const
        {
            return x == other.x && y == other.y && z == other.z && w == other.w;
        }
        // Operador de diferen�a (!=)
        bool operator!=(const vetor4& other) const
        {
            return !(*this == other);
        }
        void normalizar()
        {
            float mag = std::sqrt(x*x + y*y + z*z + w*w);
            if(mag < 0) return;
            x = x / mag;
            y = y / mag;
            w = w / mag;
            z = z / mag;
        }
        T tamanho() const
        {
            return std::sqrt(x * x + y * y + w * w + z * z);
        }
    };
}
typedef BECOMMONS_NS::vetor4<float> fvet4;
typedef BECOMMONS_NS::vetor4<double> dvet4;
typedef BECOMMONS_NS::vetor4<int> ivet4;
