inline const char* imagem_frag = R"(
#version 330 core

in vec2 Uv;
out vec4 FragColor;
uniform bool flip = false;
uniform sampler2D textura;
uniform vec4 difusa;

void main()
{
if(!flip)
    FragColor = texture(textura, vec2(Uv.x, -Uv.y)).rgba;
    else
    FragColor = texture(textura, Uv) * difusa;
}
)";