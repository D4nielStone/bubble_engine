/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
#include <cmath>
/* Defini��o da estrutura vetor3 */

namespace bubble
{
    template<class T>
    struct vetor3
    {
        T x, y, z;

        // Construtor com par�metros
        vetor3(T x, T y, T z) : x(x), y(y), z(z) {}

        // Construtor padr�o
        vetor3() : x(T{}), y(T{}), z(T{}) {}

        // Operador de soma com outro vetor3
        vetor3 operator+(const vetor3& other) const
        {
            return vetor3{ x + other.x, y + other.y, z + other.z };
        }

        // Operador de soma com um escalar
        vetor3 operator+(const T& scalar) const
        {
            return vetor3{ x + scalar, y + scalar, z + scalar };
        }

        // Operador de soma acumulada com outro vetor3
        vetor3& operator+=(const vetor3& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        // Operador de soma acumulada com um escalar
        vetor3& operator+=(const T& scalar)
        {
            x += scalar;
            y += scalar;
            z += scalar;
            return *this;
        }

        // Operador de subtra��o com outro vetor3
        vetor3 operator-(const vetor3& other) const
        {
            return vetor3{ x - other.x, y - other.y, z - other.z };
        }

        // Operador de subtra��o com um escalar
        vetor3 operator-(const T& scalar) const
        {
            return vetor3{ x - scalar, y - scalar, z - scalar };
        }

        // Operador de subtra��o acumulada com outro vetor3
        vetor3& operator-=(const vetor3& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        // Operador de subtra��o acumulada com um escalar
        vetor3& operator-=(const T& scalar)
        {
            x -= scalar;
            y -= scalar;
            z -= scalar;
            return *this;
        }

        // Operador de multiplica��o com outro vetor3
        vetor3 operator*(const vetor3& other) const
        {
            return vetor3{ x * other.x, y * other.y, z * other.z };
        }

        // Operador de multiplica��o com um escalar
        vetor3 operator*(const T& scalar) const
        {
            return vetor3{ x * scalar, y * scalar, z * scalar };
        }

        // Operador de multiplica��o acumulada com outro vetor3
        vetor3& operator*=(const vetor3& other)
        {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            return *this;
        }

        // Operador de multiplica��o acumulada com um escalar
        vetor3& operator*=(const T& scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        // Operador de igualdade
        bool operator==(const vetor3& other) const
        {
            return x == other.x && y == other.y && z == other.z;
        }

        // Operador de diferen�a
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
    };
}

typedef bubble::vetor3<float> vet3;