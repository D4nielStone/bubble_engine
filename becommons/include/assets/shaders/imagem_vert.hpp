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
 * @file imagem_vert.hpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 */
inline const char* imagem_vert = R"(

#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUv;

out vec2 Uv;

struct Quadrado
{
    vec2 tamanho;
    vec2 posicao;
};
uniform Quadrado quadrado;
uniform mat4 projecao;

void main()
{
    Uv = aUv;

    // Ajuste a posição e o tamanho do quadrado
    vec2 scaledPos = aPos * quadrado.tamanho;
    vec2 finalPos = scaledPos + quadrado.posicao;
    
    gl_Position = projecao * vec4(finalPos, 0.0, 1.0);
}
)";
