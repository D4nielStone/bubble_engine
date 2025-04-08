/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file vetor2.hpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#pragma once
#include <cmath>
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

        // Deve somar
        vetor2 operator+(const vetor2& other) const
        {
            return vetor2{ x + other.x, y + other.y };
        };
        vetor2 operator+(const float other) const
        {
            return vetor2{ x + other, y + other };
        };
        vetor2& operator+=(const vetor2& other)
        {
            x += other.x;
            y += other.y;
            return *this;
        };
        vetor2& operator+=(const float other)
        {
            x += other;
            y += other;
            return *this;
        };
        // Deve subtrair
        vetor2 operator-(const vetor2& other) const
        {
            return vetor2{ x - other.x, y - other.y };
        };
        vetor2 operator-(const float other) const
        {
            return vetor2{ x - other, y - other};
        };
        vetor2& operator-=(const vetor2& other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        };
        vetor2& operator-=(const float other)
        {
            x -= other;
            y -= other;
            return *this;
        };
        // Deve multiplicar
        vetor2 operator*(const vetor2& other)
        {
            return vetor2{ x * other.x, y * other.y };
        };
        vetor2 operator*(float other)
        {
            return vetor2{ x * other, y * other };
        };
        vetor2 operator*=(const vetor2& other)
        {
            x *= other.x;
            y *= other.y;
            return *this;
        };
        vetor2 operator*=(float other)
        {
            x *= other;
            y *= other;
            return *this;
        };
        // Deve dividir
        vetor2 operator/(const vetor2& other)
        {
            return vetor2{ x / other.x, y / other.y};
        };
        vetor2 operator/(float other)
        {
            return vetor2{ x / other, y / other };
        };
        vetor2 operator/=(const vetor2& other)
        {
            x /= other.x;
            y /= other.y;
            return *this;
        };
        vetor2 operator/=(float other)
        {
            x /= other;
            y /= other;
            return *this;
        };
        // Operador de igualdade (==)
        bool operator==(const vetor2& other) const
        {
            return x == other.x && y == other.y;
        }
        // Operador de diferen�a (!=)
        bool operator!=(const vetor2& other) const
        {
            return !(*this == other);
        }
        void normalizar()
        {
            float mag = std::sqrt(x*x + y*y);
            if(mag < 0) return;
            x = x / mag;
            y = y / mag;
        }
        T tamanho() const
        {
            return std::sqrt(x * x + y * y);
        }
    };
}

typedef bubble::vetor2<float> vet2;
