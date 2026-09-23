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
 * @file vintega_frag.hpp
 */

inline const char* vintega_frag = R"(
#version 330 core

in vec2 Uv;
out vec4 FragColor;

uniform sampler2D textura;
uniform vec2 resolucao_textura;
uniform bool flip = false;
uniform float time;           // para animação da granulação
uniform float sepiaStrength = 0.1;  // intensidade do tom sépia (0.0–1.0)
uniform float vignetteSize = 0.75;   // tamanho da vinheta (0.0–1.5)
uniform float vignetteSoft = 0.5;   // suavidade da vinheta (0.0–1.0)
uniform float noiseStrength = 0.05;  // intensidade da granulação (0.0–0.1)

// Quantiza uma cor em n-levels por canal
vec3 quantize8bit(vec3 color) {
    // 3 bits red, 3 bits green, 2 bits blue => levels = (8, 8, 4)
    vec3 levels = vec3(8.0, 8.0, 4.0);
    return floor(color * levels) / levels;
}

float rand(vec2 co) {
    // geração de ruído simples
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
    // Amostra de cor original
    vec2 uv = Uv;
    if(flip) uv.y = -uv.y;
    vec4 color = texture(textura, uv);
    
    // --- TONALIDADE SÉPIA ---
    float r = dot(color.rgb, vec3(0.393, 0.769, 0.189));
    float g = dot(color.rgb, vec3(0.349, 0.686, 0.168));
    float b = dot(color.rgb, vec3(0.272, 0.534, 0.131));
    vec3 sepia = vec3(r, g, b);
    color.rgb = mix(color.rgb, sepia, sepiaStrength);
    
    // --- VINHETA ---
    vec2 position = (Uv * resolucao_textura - 0.5 * resolucao_textura) / resolucao_textura;
    float len = length(position);
    float vignette = smoothstep(vignetteSize, vignetteSize - vignetteSoft, len);
    color.rgb *= vignette;
    
    // --- GRANULAÇÃO ---
    float noise = rand(Uv * time) * noiseStrength;
    color.rgb += noise;
    
    // --- 8‑BIT QUANTIZATION ---
    color.rgb = quantize8bit(color.rgb);
    
    FragColor = vec4(color.rgb, color.a);
}
)";
