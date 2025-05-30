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
#include <type_traits>
#include <bullet/btBulletDynamicsCommon.h>
#include "becommons_namespace.hpp"
#include "glm/glm.hpp"
/* Definição da estrutura vetor3 */

namespace BECOMMONS_NS {
    template<class T>
    struct vetor3 {
        T x, y, z;
        static_assert(std::is_arithmetic_v<T>, "vetor3<T>: T precisa ser um numero aritimético");
            
        constexpr vetor3(T x, T y, T z = T{}) : x(x), y(y), z(z) {}
        constexpr vetor3(T f = {}) : x(f), y(f), z(f) {}

        constexpr vetor3(const glm::vec<3, T, glm::packed_highp>& other)
            : x(other.x), y(other.y), z(other.z) {} 
        
        constexpr vetor3(const btVector3& other)
            : x(static_cast<T>(other.getX())),
              y(static_cast<T>(other.getY())),
              z(static_cast<T>(other.getZ())) {}
        
        glm::vec<3, T, glm::packed_highp> to_glm() const {
            return glm::vec<3, T, glm::packed_highp>((x), (y), (z));
        }
        
        btVector3 to_btvec() const {
            return btVector3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
        }

        // Deve somar
        vetor3 operator+(const vetor3& other) const
        {
            return vetor3{ x + other.x, y + other.y, z + other.z };
        };
        vetor3 operator+(const T other) const
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
        vetor3& operator+=(const T other)
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
        vetor3 operator-(const T other) const
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
        vetor3& operator-=(const T other)
        {
            x -= other;
            y -= other;
            z -= other;
            return *this;
        };
        // Deve multiplicar
        vetor3 operator*(const vetor3& other) const
        {
            return vetor3{ x * other.x, y * other.y, z * other.z };
        };
        vetor3 operator*(T other) const 
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
        vetor3 operator*=(T other)
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
        vetor3 operator/(T other)
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
        vetor3 operator/=(T other)
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
        vetor3<T> normalizar()
        {
            float mag = std::sqrt(x*x + y*y + z*z);
            if(mag < 0) return *this;
            x = x / mag;
            y = y / mag;
            z = z / mag;
            return *this;
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
