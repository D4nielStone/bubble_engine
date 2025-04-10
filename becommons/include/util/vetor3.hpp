/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file vetor3.hpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#pragma once
#include <cmath>
#include "becommons.hpp"
/* Definição da estrutura vetor3 */

namespace BECOMMONS_NS
{
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

typedef BECOMMONS_NS::vetor3<float> vet3;
