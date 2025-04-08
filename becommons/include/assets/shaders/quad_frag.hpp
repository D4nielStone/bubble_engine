/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file quad_frag.hpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 */
inline const char* quad_frag = 

R"(
#version 330 core

in vec2 Uv;

uniform vec4 cor;
out vec4 FragColor;
uniform vec2 resolucao_textura;
uniform bool mostrar_bordas;
uniform int tamanho_bordas; // Espessura da borda em px
uniform vec4 cor_borda; // Cor da borda

void main()
{
    float bordax = tamanho_bordas / resolucao_textura.x;
    float borday = tamanho_bordas / resolucao_textura.y;
    // Detectar borda com base nas coordenadas UV
    if (mostrar_bordas)
    {
        if (Uv.x < bordax || Uv.x > 1.0 - bordax || Uv.y < borday || Uv.y > 1.0 - borday)
        {
            FragColor = cor_borda; // Cor da borda
            return;
        }
    }

    // Define a cor final do fragmento
    FragColor = cor;
}
)";
