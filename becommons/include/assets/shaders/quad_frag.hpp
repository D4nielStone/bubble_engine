inline const char* quad_frag = 
R"(
#version 330 core

uniform vec4 cor;
out vec4 FragColor;

void main()
{
    FragColor = cor;
}
)";