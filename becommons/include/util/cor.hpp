/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
/* Defini��o da estrutura cor */
namespace bubble
{
    struct cor
    {
        float r = 1.f, g = 1.f, b = 1.f, a = 1.f;
        cor(const float& f) : r(f), g(f), b(f), a(1.f) {}
        cor(const float& r, const float& g, const float& b, const float& a) : r(r), g(g), b(b), a(a) {};
        cor(const float& r, const float& g, const float& b) : r(r), g(g), b(b) {};
        cor() = default;
        // Operador de soma (+)
        cor operator+(const float other) const
        {
            return { r + other, g + other, b + other, 1.f };
        }
        // Operador de igualdade (==)
        bool operator==(const cor& other) const
        {
            return r == other.r && g == other.g && b == other.b && a == other.a;
        }
        // Operador de diferen�a (!=)
        bool operator!=(const cor& other) const
        {
            return !(*this == other);
        }
    };
}
