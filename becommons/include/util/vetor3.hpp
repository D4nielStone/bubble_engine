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
 * @file vetor3.hpp
 */

#pragma once
#include <cmath>
#include "becommons_namespace.hpp"
/* Definição da estrutura vetor3 */

namespace BECOMMONS_NS {
    template<class T>
    struct vetor3
    {
        T x, y, z;

        // Construtor com parâmetros
        vetor3(T x, T y, T z) : x(x), y(y), z(z) {}

        // Construtor padrão
        vetor3() : x(T{}), y(T{}), z(T{}) {}

        // Deve somar
        vetor3 operator+(const vetor3& other) const
        {
            return vetor3{ x + other.x, y + other.y, z + other.z };
        };
        vetor3 operator+(const float other) const
        {
            return vetor3{ x + other, y + other, z + other };
        };
        vetor3& operator+=(const vetor3& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        };
        vetor3& operator+=(const float other)
        {
            x += other;
            y += other;
            z += other;
            return *this;
        };
        // Deve subtrair
        vetor3 operator-(const vetor3& other) const
        {
            return vetor3{ x - other.x, y - other.y, z - other.z };
        };
        vetor3 operator-(const float other) const
        {
            return vetor3{ x - other, y - other, z - other };
        };
        vetor3& operator-=(const vetor3& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        };
        vetor3& operator-=(const float other)
        {
            x -= other;
            y -= other;
            z -= other;
            return *this;
        };
        // Deve multiplicar
        vetor3 operator*(const vetor3& other)
        {
            return vetor3{ x * other.x, y * other.y, z * other.z };
        };
        vetor3 operator*(float other)
        {
            return vetor3{ x * other, y * other, z * other };
        };
        vetor3 operator*=(const vetor3& other)
        {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            return *this;
        };
        vetor3 operator*=(float other)
        {
            x *= other;
            y *= other;
            z *= other;
            return *this;
        };
        // Deve dividir
        vetor3 operator/(const vetor3& other)
        {
            return vetor3{ x / other.x, y / other.y, z / other.z};
        };
        vetor3 operator/(float other)
        {
            return vetor3{ x / other, y / other, z / other };
        };
        vetor3 operator/=(const vetor3& other)
        {
            x /= other.x;
            y /= other.y;
            z /= other.z;
            return *this;
        };
        vetor3 operator/=(float other)
        {
            x /= other;
            y /= other;
            z /= other;
            return *this;
        };
        // Operador de igualdade (==)
        bool operator==(const vetor3& other) const
        {
            return x == other.x && y == other.y && z == other.z;
        }
        // Operador de diferen�a (!=)
        bool operator!=(const vetor3& other) const
        {
            return !(*this == other);
        }
        void normalizar()
        {
            float mag = std::sqrt(x*x + y*y + z*z);
            if(mag < 0) return;
            x = x / mag;
            y = y / mag;
            z = z / mag;
        }
        T tamanho() const
        {
            return std::sqrt(x * x + y * y + z * z);
        }
    };
}

typedef BECOMMONS_NS::vetor3<float> fvet3;
typedef BECOMMONS_NS::vetor3<double> dvet3;
typedef BECOMMONS_NS::vetor3<int> ivet3;
