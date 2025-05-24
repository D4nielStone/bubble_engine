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
#include <glm/glm.hpp>
#include "becommons_namespace.hpp"
#include <btBulletDynamicsCommon.h>
/* Definição da estrutura vetor4 */

namespace BECOMMONS_NS {
    template<class T>
    struct vetor4
    {
        T x, y, z, w;

        static_assert(std::is_arithmetic_v<T>, "vetor4<T>: T precisa ser um numero aritimético");
            
        constexpr vetor4(T x = T{}, T y = T{}, T z = T{}, T w = T{}) : x(x), y(y), z(z), w(w) {}
        constexpr vetor4(T f) : x(f), y(f), z(f), w(f) {}

        constexpr vetor4(const glm::vec<4, T, glm::packed_highp> other)
            : x(other.x), y(other.y), z(other.z), w(other.w) {} 
        
        constexpr vetor4(const btQuaternion& other)
            : x(static_cast<T>(other.getX())),
              y(static_cast<T>(other.getY())),
              z(static_cast<T>(other.getZ())),
              w(static_cast<T>(other.getW())) {}
        
        glm::vec<4, T, glm::packed_highp> to_glm() const {
            return glm::vec<4, T, glm::packed_highp>(x, y, z, w);
        }

        btQuaternion to_btvec() const {
            return btQuaternion(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(w));
        }

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
        vetor4<T> normalizar()
        {
            float mag = std::sqrt(x*x + y*y + z*z + w*w);
            if(mag < 0) return *this;
            x = x / mag;
            y = y / mag;
            w = w / mag;
            z = z / mag;
            return *this;
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
