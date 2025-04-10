/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file tempo.hpp
 * @brief Inicia e gerencia o tempo
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 *
 */

#pragma once
#include <thread>
#include <chrono>
#include "becommons.hpp"
namespace BECOMMONS_NS
{
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
