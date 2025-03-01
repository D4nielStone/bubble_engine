
/** @copyright Copyright (c) 2025 Daniel Oliveira */

inline const char* quad_vert =
R"(
#version 330 core

uniform vec2 posicoes[256]
uniform mat4 projecao;

void main()
{
gl_Position = vec4(posicoes[gl_InstanceID], 0.0, 1.0);
}
)";