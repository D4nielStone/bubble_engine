/** @copyright 
MIT License
Copyright (c) 2025 Daniel Oliveira

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
*/
/**
 * @file luz_direcional.hpp
 */

#pragma once
#include "componente.hpp"
#include "util/vetor3.hpp"
#include "util/cor.hpp"

namespace becommons {
    struct luz_direcional : public componente
    {
        fvet3 direcao, ambiente, cor; float intensidade = 1.f;
        explicit  luz_direcional(fvet3 direcao = {-1.f, -1.f, -1.f}, fvet3 ambiente = {0.1f, 0.1f, 0.1f}, fvet3 cor = {1.f, 1.f, 1.f}, float intensidade = 1.f) :
        direcao(direcao), ambiente(ambiente), cor(cor), intensidade(intensidade){}
        bool serializar(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const override;
        bool analizar(const rapidjson::Value& value) override;
        static constexpr mascara mascara = {COMPONENTE_LUZ_DIRECIONAL};
    };
}
