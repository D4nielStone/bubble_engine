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
 * @file tempo.hpp
 * @brief Inicia e gerencia o tempo
 */

#pragma once
#include <thread>
#include <chrono>
#include "becommons_namespace.hpp"
namespace BECOMMONS_NS {
    struct tempo {
        tempo() : tempoCorrido(std::chrono::high_resolution_clock::now()) {}

        // Calcula e retorna o delta time em segundos
        double obterDeltaTime() {
            return delta.count();
        }
        void calcularDT()
        {
            auto tempoAtual = std::chrono::high_resolution_clock::now();
            delta = tempoAtual - tempoCorrido;
            tempoCorrido = tempoAtual; // Atualiza o �ltimo tempo
        }

        std::chrono::duration<double> delta;
        std::chrono::time_point<std::chrono::high_resolution_clock> tempoCorrido;
    };
}
