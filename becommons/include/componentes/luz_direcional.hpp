/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
#include "componente.hpp"
#include "util/vetor3.hpp"

namespace bubble
{
    struct luz_direcional : public componente
    {
        vet3 direcao, ambiente, cor; float intensidade = 1.f;
        luz_direcional() = default;
        explicit  luz_direcional(vet3 direcao, vet3 ambiente, vet3 cor, float intensidade) :
        direcao(direcao), ambiente(ambiente), cor(cor), intensidade(intensidade)
        {

        }
        static constexpr mascara mascara = {COMPONENTE_LUZ_DIRECIONAL};
    };
}
