/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file skybox_vert.hpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 */


inline const char* skybox_vert = 
R"(
#version 330 core
layout (location = 0) in vec3 aPos;
out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    vec4 pos = projection * view * vec4(-aPos, 1.0);
    gl_Position = pos.xyww;      
    TexCoords = aPos;
})";
