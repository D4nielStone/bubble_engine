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
 * @file directional_light.hpp
 */

#pragma once
#include "commons_namespace.hpp"
#include "component.hpp"
#include "util/vector3.hpp"
#include "util/color.hpp"

namespace COMMONS_NS {
    struct directional_light : public component
    {
        fvector_type3 direction, ambient, color; float intensity = 1.f;
        directional_light() = default;
        explicit  directional_light(fvector_type3 direction, fvector_type3 ambient, fvector_type3 color, float intensity) :
        direction(direction), ambient(ambient), color(color), intensity(intensity){}
        bool serialize(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const override;
        bool analyze(const rapidjson::Value& value) override;
        static constexpr mask mask = {COMPONENTE_LUZ_DIRECIONAL};
    };
}
