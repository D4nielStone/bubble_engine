/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
#include "componente.hpp"
#include "util/vetor3.hpp"

namespace bubble
{
    struct luz_pontual : public componente
    {
        vet3 position;
        vet3 color;
        float intensity;
        float constant;
        float linear;
        float quadratic;
        explicit  luz_pontual(vet3 position = vet3(0, 2, -2), vet3 color = vet3(1, 1, 1), float intensity = 5.f , float constant = 1.f, float linear = 0.09f, float quadratic = 0.032f) :
            position(position), color(color), intensity(intensity), constant(constant), linear(linear), quadratic(quadratic)
        {}
        static constexpr mascara mascara = {COMPONENTE_LUZ_PONTUAL};
    };
}
