/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
/* Defini��o da estrutura vetor2 */

namespace bubble
{
    template<class T>
    struct vetor2
    {
        T x, y;

        // Construtor com par�metros
        vetor2(T x, T y) : x(x), y(y) {}

        // Construtor padr�o
        vetor2() : x(T{}), y(T{}) {}

        // Operador de soma com outro vetor2
        vetor2 operator+(const vetor2& other) const
        {
            return vetor2{ x + other.x, y + other.y };
        }

        // Operador de soma com um escalar
        vetor2 operator+(const T& scalar) const
        {
            return vetor2{ x + scalar, y + scalar };
        }

        // Operador de soma acumulada com outro vetor2
        vetor2& operator+=(const vetor2& other)
        {
            x += other.x;
            y += other.y;

            return *this;
        }

        // Operador de soma acumulada com um escalar
        vetor2& operator+=(const T& scalar)
        {
            x += scalar;
            y += scalar;
            return *this;
        }

        // Operador de subtra��o com outro vetor2
        vetor2 operator-(const vetor2& other) const
        {
            return vetor2{ x - other.x, y - other.y };
        }

        // Operador de subtra��o com um escalar
        vetor2 operator-(const T& scalar) const
        {
            return vetor2{ x - scalar, y - scalar };
        }

        // Operador de subtra��o acumulada com outro vetor2
        vetor2& operator-=(const vetor2& other)
        {
            x -= other.x;
            y -= other.y;

            return *this;
        }

        // Operador de subtra��o acumulada com um escalar
        vetor2& operator-=(const T& scalar)
        {
            x -= scalar;
            y -= scalar;
            return *this;
        }

        // Operador de multiplica��o com outro vetor2
        vetor2 operator*(const vetor2& other) const
        {
            return vetor2{ x * other.x, y * other.y };
        }

        // Operador de multiplica��o com um escalar
        vetor2 operator*(const T& scalar) const
        {
            return vetor2{ x * scalar, y * scalar };
        }

        // Operador de multiplica��o acumulada com outro vetor2
        vetor2& operator*=(const vetor2& other)
        {
            x *= other.x;
            y *= other.y;

            return *this;
        }

        // Operador de multiplica��o acumulada com um escalar
        vetor2& operator*=(const T& scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        // Operador de igualdade
        bool operator==(const vetor2& other) const
        {
            return x == other.x && y == other.y;
        }

        // Operador de diferen�a
        bool operator!=(const vetor2& other) const
        {
            return !(*this == other);
        }

    };
}

typedef bubble::vetor2<double> vet2;