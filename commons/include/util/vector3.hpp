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
 * @file vector3.hpp
 */

#pragma once
#include <cmath>
#include <type_traits>
#include "btBulletDynamicsCommon.h"
#include "commons_namespace.hpp"
#include "glm/glm.hpp"
/* Definition da estrutura vector3 */

namespace COMMONS_NS {
    template<class T>
    struct vector3 {
        T x, y, z;
        static_assert(std::is_arithmetic_v<T>, "vector3<T>: T needs ser um number arithmetic");

        constexpr vector3(T x, T y, T z = T{}) : x(x), y(y), z(z) {}
        constexpr vector3(T f = {}) : x(f), y(f), z(f) {}

        constexpr vector3(const glm::vec<3, T, glm::packed_highp>& other)
            : x(other.x), y(other.y), z(other.z) {}

        constexpr vector3(const btVector3& other)
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
        vector3 operator+(const vector3& other) const
        {
            return vector3{ x + other.x, y + other.y, z + other.z };
        };
        vector3 operator+(const T other) const
        {
            return vector3{ x + other, y + other, z + other };
        };
        vector3& operator+=(const vector3& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        };
        vector3& operator+=(const T other)
        {
            x += other;
            y += other;
            z += other;
            return *this;
        };
        // Deve subtrair
        vector3 operator-(const vector3& other) const
        {
            return vector3{ x - other.x, y - other.y, z - other.z };
        };
        vector3 operator-(const T other) const
        {
            return vector3{ x - other, y - other, z - other };
        };
        vector3& operator-=(const vector3& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        };
        vector3& operator-=(const T other)
        {
            x -= other;
            y -= other;
            z -= other;
            return *this;
        };
        // Deve multiplicar
        vector3 operator*(const vector3& other) const
        {
            return vector3{ x * other.x, y * other.y, z * other.z };
        };
        vector3 operator*(T other) const
        {
            return vector3{ x * other, y * other, z * other };
        };
        vector3 operator*=(const vector3& other)
        {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            return *this;
        };
        vector3 operator*=(T other)
        {
            x *= other;
            y *= other;
            z *= other;
            return *this;
        };
        // Deve dividir
        vector3 operator/(const vector3& other)
        {
            return vector3{ x / other.x, y / other.y, z / other.z};
        };
        vector3 operator/(T other)
        {
            return vector3{ x / other, y / other, z / other };
        };
        vector3 operator/=(const vector3& other)
        {
            x /= other.x;
            y /= other.y;
            z /= other.z;
            return *this;
        };
        vector3 operator/=(T other)
        {
            x /= other;
            y /= other;
            z /= other;
            return *this;
        };
        // Operador de igualdade (==)
        bool operator==(const vector3& other) const
        {
            return x == other.x && y == other.y && z == other.z;
        }
        // Operador de diferen�a (!=)
        bool operator!=(const vector3& other) const
        {
            return !(*this == other);
        }
        vector3<T> normalize()
        {
            float mag = std::sqrt(x*x + y*y + z*z);
            if(mag < 0) return *this;
            x = x / mag;
            y = y / mag;
            z = z / mag;
            return *this;
        }
        T size() const
        {
            return std::sqrt(x * x + y * y + z * z);
        }
    };
}

typedef COMMONS_NS::vector3<float> fvector_type3;
typedef COMMONS_NS::vector3<double> dvector_type3;
typedef COMMONS_NS::vector3<int> ivector_type3;
