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
 * @file time.hpp
 * @brief Inicia e gerencia o time
 */

#pragma once
#include <thread>
#include <chrono>
#include "commons_namespace.hpp"
namespace COMMONS_NS {
    struct time {
        time() : elapsed_time(std::chrono::high_resolution_clock::now()) {}

        // Calcula e retorna o delta time em segundos
        double get_delta_time() {
            return delta.count();
        }
        void calculateDT()
        {
            auto haspoAtual = std::chrono::high_resolution_clock::now();
            delta = haspoAtual - elapsed_time;
            elapsed_time = haspoAtual; // Atualiza o �ltimo time
        }

        std::chrono::duration<double> delta;
        std::chrono::time_point<std::chrono::high_resolution_clock> elapsed_time;
    };
}
