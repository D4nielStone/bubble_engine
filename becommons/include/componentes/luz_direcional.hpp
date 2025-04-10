/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file luz_direcional.hpp
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
#include "util/cor.hpp"

namespace BECOMMONS_NS {
    struct luz_direcional : public componente
    {
        fvet3 direcao, ambiente, cor; float intensidade = 1.f;
        luz_direcional() = default;
        explicit  luz_direcional(fvet3 direcao, fvet3 ambiente, fvet3 cor, float intensidade) :
        direcao(direcao), ambiente(ambiente), cor(cor), intensidade(intensidade){}
        bool serializar(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const override;
        bool analizar(const rapidjson::Value& value) override;
        static constexpr mascara mascara = {COMPONENTE_LUZ_DIRECIONAL};
    };
}
