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
 * @file vector4.hpp
 */

#pragma once
#include <cmath>
#include <glm/glm.hpp>
#include "commons_namespace.hpp"
#include <btBulletDynamicsCommon.h>
/* Definition da estrutura vector4 */

namespace COMMONS_NS {
    template<class T>
    struct vector4
    {
        T x, y, z, w;

        static_assert(std::is_arithmetic_v<T>, "vector4<T>: T needs ser um number arithmetic");

        constexpr vector4(T x = T{}, T y = T{}, T z = T{}, T w = T{}) : x(x), y(y), z(z), w(w) {}
        constexpr vector4(T f) : x(f), y(f), z(f), w(f) {}

        constexpr vector4(const glm::vec<4, T, glm::packed_highp> other)
            : x(other.x), y(other.y), z(other.z), w(other.w) {}

        constexpr vector4(const btQuaternion& other)
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
        vector4 operator+(const vector4& other) const
        {
            return vector4{ x + other.x, y + other.y, z + other.z, w + other.w };
        };
        vector4 operator+(const float other) const
        {
            return vector4{ x + other, y + other, z + other, w + other };
        };
        vector4& operator+=(const vector4& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;
            return *this;
        };
        vector4& operator+=(const float other)
        {
            x += other;
            y += other;
            z += other;
            w += other;
            return *this;
        };
        // Deve subtrair
        vector4 operator-(const vector4& other) const
        {
            return vector4{ x - other.x, y - other.y, z - other.z, w - other.w };
        };
        vector4 operator-(const float other) const
        {
            return vector4{ x - other, y - other, z - other, w - other };
        };
        vector4& operator-=(const vector4& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            w -= other.w;
            return *this;
        };
        vector4& operator-=(const float other)
        {
            x -= other;
            y -= other;
            z -= other;
            w -= other;
            return *this;
        };
        // Deve multiplicar
        vector4 operator*(const vector4& other)
        {
            return vector4{ x * other.x, y * other.y, z * other.z, w * other.w };
        };
        vector4 operator*(float other)
        {
            return vector4{ x * other, y * other, z * other, w * other };
        };
        vector4 operator*=(const vector4& other)
        {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            w *= other.w;
            return *this;
        };
        vector4 operator*=(float other)
        {
            x *= other;
            y *= other;
            z *= other;
            w *= other;
            return *this;
        };
        // Deve dividir
        vector4 operator/(const vector4& other)
        {
            return vector4{ x / other.x, y / other.y, z / other.z, w / other.w };
        };
        vector4 operator/(float other)
        {
            return vector4{ x / other, y / other, z / other, w / other };
        };
        vector4 operator/=(const vector4& other)
        {
            x /= other.x;
            y /= other.y;
            z /= other.z;
            w /= other.w;
            return *this;
        };
        vector4 operator/=(float other)
        {
            x /= other;
            y /= other;
            z /= other;
            w /= other;
            return *this;
        };
        // Operador de igualdade (==)
        bool operator==(const vector4& other) const
        {
            return x == other.x && y == other.y && z == other.z && w == other.w;
        }
        // Operador de diferen�a (!=)
        bool operator!=(const vector4& other) const
        {
            return !(*this == other);
        }
        vector4<T> normalize()
        {
            float mag = std::sqrt(x*x + y*y + z*z + w*w);
            if(mag < 0) return *this;
            x = x / mag;
            y = y / mag;
            w = w / mag;
            z = z / mag;
            return *this;
        }
        T size() const
        {
            return std::sqrt(x * x + y * y + w * w + z * z);
        }
    };
}
typedef COMMONS_NS::vector4<float> fvector_type4;
typedef COMMONS_NS::vector4<double> dvector_type4;
typedef COMMONS_NS::vector4<int> ivector_type4;
