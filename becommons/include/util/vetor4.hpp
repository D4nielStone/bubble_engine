/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
/* Defini��o da estrutura vetor4 */

namespace bubble
{
    template<class T>
    struct vetor4
    {
        T x, y, w, h;

        vetor4(T x, T y, T w, T h) : x(x), y(y), w(w), h(h) {}
        vetor4() = default;
        // Deve somar
        vetor4 operator+(const vetor4& other) const
        {
            return vetor4{ x + other.x, y + other.y, static_cast<int>(w + other.w), static_cast<int>(h + other.h) };
        };
        vetor4 operator+(const float other) const
        {
            return vetor4{ x + other, y + other, static_cast<int>(w + other), static_cast<int>(h + other) };
        };
        vetor4& operator+=(const vetor4& other)
        {
            x += other.x;
            y += other.y;
            return *this;
        };
        vetor4& operator+=(const float other)
        {
            x += other;
            y += other;
            return *this;
        };
        // Deve subtrair
        vetor4 operator-(const vetor4& other) const
        {
            return vetor4{ x - other.x, y - other.y, static_cast<int>(w - other.w), static_cast<int>(h - other.h) };
        };
        vetor4 operator-(const float other) const
        {
            return vetor4{ x - other, y - other, static_cast<int>(w - other), static_cast<int>(h - other) };
        };
        vetor4& operator-=(const vetor4& other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        };
        vetor4& operator-=(const float other)
        {
            x -= other;
            y -= other;
            return *this;
        };
        // Deve multiplicar
        vetor4 operator*(const vetor4& other)
        {
            return vetor4{ x * other.x, y * other.y, static_cast<int>(w * other.w), static_cast<int>(h * other.h) };
        };
        vetor4 operator*(float other)
        {
            return vetor4{ x * other, y * other, static_cast<int>(w * other), static_cast<int>(h * other) };
        };
        vetor4 operator*=(const vetor4& other)
        {
            x *= other.x;
            y *= other.y;
            return *this;
        };
        vetor4 operator*=(float other)
        {
            x *= other;
            y *= other;
            return *this;
        };
        // Deve dividir
        vetor4 operator/(const vetor4& other)
        {
            return vetor4{ x / other.x, y / other.y, static_cast<int>(w / other.w), static_cast<int>(h / other.h) };
        };
        vetor4 operator/(float other)
        {
            return vetor4{ x / other, y / other, static_cast<int>(w / other), static_cast<int>(h / other) };
        };
        vetor4 operator/=(const vetor4& other)
        {
            x /= other.x;
            y /= other.y;
            return *this;
        };
        vetor4 operator/=(float other)
        {
            x /= other;
            y /= other;
            return *this;
        };
        // Operador de igualdade (==)
        bool operator==(const vetor4& other) const
        {
            return x == other.x && y == other.y && w == other.w && h == other.h;
        }
        // Operador de diferen�a (!=)
        bool operator!=(const vetor4& other) const
        {
            return !(*this == other);
        }
    };
}