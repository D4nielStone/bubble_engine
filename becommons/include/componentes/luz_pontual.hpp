/** @copyright 
MIT LicenseCopyright (c) 2025 Daniel Oliveira

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
 * @file luz_pontual.hpp
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
