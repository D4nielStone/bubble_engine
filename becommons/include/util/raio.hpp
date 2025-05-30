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
 * @file raio.hpp
 */

#pragma once
#include <glm/glm.hpp>
#include "vetor3.hpp"
#include <bullet/btBulletDynamicsCommon.h>
#include "becommons_namespace.hpp"

namespace BECOMMONS_NS {
    // Estrutura para armazenar os resultados do Raycast
    struct raio
    {
        fvet3 origem;
        fvet3 direcao;
    };
    struct resultadoRaio {
        bool atingiu;                      // Se o raio atingiu algo
        fvet3 pontoDeColisao;            // Ponto de colis�o
        fvet3 normalAtingida;           // Normal da superf�cie atingida
        const btCollisionObject* objetoAtingido; // Objeto atingido
    };
}
