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
 * @file imagem_frag.hpp
 */
inline const char* imagem_frag = R"(

#version 330 core

in vec2 Uv;
out vec4 FragColor;

uniform bool flip = false;
uniform vec4 quadrado;
uniform sampler2D textura;
uniform int tamanho_bordas; // Espessura da borda em px
uniform vec4 cor_borda; // Cor da borda

void main()
{
    vec2 nUv = flip ? vec2(Uv.x, -Uv.y) : Uv;
    vec4 cor_textura = texture(textura, nUv);

    vec2 resolucao_textura = vec2(quadrado.z, quadrado.w);
    float bordax = tamanho_bordas / resolucao_textura.x;
    float borday = tamanho_bordas / resolucao_textura.y;
    // Detectar borda com base nas coordenadas UV
    if (cor_borda.w != 0)
    {
        if (Uv.x < bordax || Uv.x > 1.0 - bordax || Uv.y < borday || Uv.y > 1.0 - borday)
        {
            FragColor = cor_borda; // Cor da borda
            return;
        }
    }
    FragColor = cor_textura;
}
)"; 
