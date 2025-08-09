/** @copyright 
 * MIT License
 * Copyright (c) 2025 Daniel Oliveira
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 * @file direcional.hpp
 */

#pragma once
#include "luz.hpp"
#include <cmath>
    
namespace iluminacao {
    struct holofote : public luz {
        fvet3 m_posicao { 0.f };
        fvet3 m_direcao { 0.f, -1.f, 0.f };
        float m_cutoff cos(12.5f * (3.1415f / 180.f));
        float m_outerCutoff cos(15.0f * (3.1415f / 180.f));

        holofote() : luz(tipo::holofote) {/*...*/}
    };
}
