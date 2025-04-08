/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file skybox_frag.hpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 */


inline const char* skybox_frag =
R"(
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, vec3(TexCoords.x, -TexCoords.y, TexCoords.z));
})";
