#include "componentes/terreno.hpp"
#include "arquivadores/imageloader.hpp"
#include "depuracao/debug.hpp"
#include "util/vertice.hpp"
#include "util/vetor3.hpp"
#include <iostream>

void bubble::terreno::carregarHeightMap(unsigned char* dados, int largura, int altura)
{
    heightmap = std::vector<std::vector<float>>(altura, std::vector<float>(largura));
    for (int j = 0; j < altura; j++)
    {
        for (int i = 0; i < largura; i++)
        {
            // Cada pixel tem 4 bytes (ARGB). 
            // Acessa o canal desejado (ex: Red = índice 1, Alpha = índice 0)
            int indiceARGB = (j * largura + i) * 4; // Base para os 4 canais
            heightmap[j][i] = dados[indiceARGB + 1] / 255.0f; // Usando canal Red (substitua +1 pelo canal desejado)
        }
    }

        std::vector<bubble::vertice> vertices;
    std::vector<unsigned int> indices;

    // Criar vértices do terreno
    for (int j = 0; j < altura; j++)
    {
        for (int i = 0; i < largura; i++)
        {
            vertice v;
            v.posicao = vet3(i, heightmap[j][i], j);
            v.uvcoords = bubble::vetor2<float>(i / (float)largura, j / (float)altura);
        
            vertices.push_back(v);
        }
    }

    // Criar índices para triângulos
    for (int j = 0; j < altura - 1; j++)
    {
        for (int i = 0; i < largura - 1; i++)
        {
            int topLeft = j * largura + i;
            int topRight = topLeft + 1;
            int bottomLeft = (j + 1) * largura + i;
            int bottomRight = bottomLeft + 1;

            // Triângulo 1
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // Triângulo 2
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    // Inicializar as normais como (0, 0, 0)
    for (auto &v : vertices)
    {   
    v.normal = vet3(0.0f, 0.0f, 0.0f);
    }

    // Percorrer os triângulos e calcular as normais das faces
    for (size_t i = 0; i < indices.size(); i += 3)
    {
    int i1 = indices[i];
    int i2 = indices[i + 1];
    int i3 = indices[i + 2];

    glm::vec3 v1 = {vertices[i1].posicao.x,vertices[i1].posicao.y,vertices[i1].posicao.z};
    glm::vec3 v2 = {vertices[i2].posicao.x,vertices[i2].posicao.y,vertices[i2].posicao.z};
    glm::vec3 v3 = {vertices[i3].posicao.x,vertices[i3].posicao.y,vertices[i3].posicao.z};

    glm::vec3 _normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
    vet3 normal = vet3(
        _normal.x,
        _normal.y,
        _normal.z
    );

    // Adicionar a normal ao vértice (antes de normalizar no final)
    vertices[i1].normal += normal;
    vertices[i2].normal += normal;
    vertices[i3].normal += normal;
    }

    // Normalizar todas as normais dos vértices
    for (auto &v : vertices)
    {
        v.normal.normalizar();
    }
    // Criar a malha
    _Mmalha.vertices = vertices;
    _Mmalha.indices = indices;
    _Mmalha.definirBuffers(); // Configura VBO, VAO, EBO
}

// Construtor do terreno
bubble::terreno::terreno(const std::string &path) : diretorio(path)
{
    // Carregar imagem como Heightmap
    bubble::imageLoader imagem(path);
    largura = imagem.obterLargura();
    altura = imagem.obterAltura();
    carregarHeightMap(imagem.obterDados(), largura, altura);
}

// Método para desenhar o terreno
void bubble::terreno::desenhar(bubble::shader &_shader)
{
    _Mmalha.desenhar(_shader);
}