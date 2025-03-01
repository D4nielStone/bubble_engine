inline const char* imagem_vert = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUv;

out vec2 Uv;

struct Quadrado
{
    vec2 tamanho;
    vec2 posicao;
};
uniform Quadrado quadrado;
uniform mat4 projecao;

void main()
{
    Uv = aUv;

    // Ajuste a posição e o tamanho do quadrado
    vec2 scaledPos = aPos * quadrado.tamanho;
    vec2 finalPos = scaledPos + quadrado.posicao;
    
    gl_Position = projecao * vec4(finalPos, 0.0, 1.0);
}
)";