#pragma once
inline const char* sombra_vert = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 modelo;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * modelo * vec4(aPos, 1.0);
}
)";
