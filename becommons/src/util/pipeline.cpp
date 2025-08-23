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
 * @file pipeline.cpp
 */

#include "util/pipeline.hpp"
#include <algorithm>

using namespace becommons;

pipeline::pipeline() {}

void pipeline::addDrawCall(const unsigned int layer, draw_call* ref) {
    m_draw_calls[layer].push_back(ref);
}

void pipeline::remDrawCall(draw_call* ref) {
    for (auto& [layer, calls] : m_draw_calls) {
        calls.erase(std::remove(calls.begin(), calls.end(), ref), calls.end());
    }
}

void pipeline::limparLayer(const unsigned int layer) {
    m_draw_calls[layer].clear();
}

void pipeline::limpar() {
    m_draw_calls.clear();
}

void pipeline::chamarDrawCall(draw_call* dc) {
    if (dc) {
        dc->m_material->usar(*dc->m_shader);
        glBindVertexArray(dc->m_vao);
        glDrawElements(dc->m_mode, dc->m_count, dc->m_type, dc->m_indices);
    }
}
void pipeline::chamarDrawCalls() {
    for (auto& [layer, calls] : m_draw_calls) {
        for (auto* dc : calls) {
            chamarDrawCall(dc);
        }
    }
}
