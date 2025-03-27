
/** @copyright Copyright (c) 2025 Daniel Oliveira */

inline const char* quad_vert =
R"(
#version 330 core
layout (location = 0) in vec4 aPos;

uniform mat4 projecao;

void main()
{
gl_Position = projecao * aPos;
}
)";
