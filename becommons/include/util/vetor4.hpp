/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
#include <cmath>
/* Defini��o da estrutura vetor4 */

namespace bubble
{
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
typedef bubble::vetor4<float> vet4;