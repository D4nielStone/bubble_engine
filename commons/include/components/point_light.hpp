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
 * @file point_light.hpp
 */

#pragma once
#include "commons_namespace.hpp"
#include "component.hpp"
#include "util/vector3.hpp"

namespace COMMONS_NS {
    struct point_light : public component
    {
        fvector_type3 position;
        fvector_type3 color;
        float intensity;
        float constant;
        float linear;
        float quadratic;
        explicit  point_light(fvector_type3 position = fvector_type3(0, 2, -2), fvector_type3 color = fvector_type3(1, 1, 1), float intensity = 5.f , float constant = 1.f, float linear = 0.09f, float quadratic = 0.032f) :
            position(position), color(color), intensity(intensity), constant(constant), linear(linear), quadratic(quadratic)
        {}
        static constexpr mask mask = {COMPONENTE_LUZ_PONTUAL};
    };
}
