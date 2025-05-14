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
 * @file terreno.hpp
 */

#pragma once
#include <vector>
#include <string>
#include "becommons_namespace.hpp"
#include "componente.hpp"
#include "util/malha.hpp"
#include "arquivadores/shader.hpp"

namespace BECOMMONS_NS {
    struct terreno : public componente
    {
        static constexpr mascara mascara = {COMPONENTE_TERRENO};
        terreno() = default;
        void carregarHeightMap(unsigned char *dados, int largura, int altura);
        explicit terreno(const std::string &path);
        void desenhar(shader& _shader);
        shader& obterShader() 
        {
            return m_shader;
        }
        malha m_malha;
        private:
            shader m_shader;
            std::string diretorio;
            std::vector<std::vector<float>> heightmap;
            int largura{0}, altura{0};
    };
}
