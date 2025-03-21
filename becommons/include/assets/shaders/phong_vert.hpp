
inline const char* phong_vert = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

out vec3 Normal;
out vec3 Position;
out vec2 Uv;

uniform mat4 modelo;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    Uv = aUV;
    Normal = mat3(transpose(inverse(modelo))) * aNormal;
    Position = vec3(modelo * vec4(aPos, 1.0)); 
    gl_Position = projection * view * modelo * vec4(aPos, 1.f);
} 
)";
