/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file luz_pontual.hpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#pragma once
#include "namespace.hpp"
#include "componente.hpp"
#include "util/vetor3.hpp"

namespace BECOMMONS_NS {
    struct luz_pontual : public componente
    {
        fvet3 position;
        fvet3 color;
        float intensity;
        float constant;
        float linear;
        float quadratic;
        explicit  luz_pontual(fvet3 position = fvet3(0, 2, -2), fvet3 color = fvet3(1, 1, 1), float intensity = 5.f , float constant = 1.f, float linear = 0.09f, float quadratic = 0.032f) :
            position(position), color(color), intensity(intensity), constant(constant), linear(linear), quadratic(quadratic)
        {}
        static constexpr mascara mascara = {COMPONENTE_LUZ_PONTUAL};
    };
}
