
/** @copyright 
MIT License
Copyright (c) 2025 Daniel Oliveira

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
*/
/**
 * @file terreno.cpp
 */

#include "componentes/terreno.hpp"
#include "arquivadores/imageloader.hpp"
#include "depuracao/debug.hpp"
#include "util/vertice.hpp"
#include "util/vetor3.hpp"
#include <iostream>

using namespace BECOMMONS_NS;

bool terreno::analizar(const rapidjson::Value& value) {
    if(value.HasMember("heightmap") && value["heightmap"].IsString())
        diretorio = value["heightmap"].GetString();
    // Carregar imagem como Heightmap
    imageLoader imagem(diretorio);
    largura = imagem.obterLargura();
    altura = imagem.obterAltura();
    gerarHeightMap(imagem.obterDados(), largura, altura);
    return true;
}
        
bool terreno::serializar(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const {
    value.AddMember("heightmap", rapidjson::Value(diretorio.c_str(), allocator), allocator);
    return true;
}

void terreno::gerarHeightMap(unsigned char* dados, int largura, int altura)
{
    heightmap = std::vector<std::vector<float>>(altura, std::vector<float>(largura));
    for (int j = 0; j < altura; j++)
    {
        for (int i = 0; i < largura; i++)
        {
            // Cada pixel tem 4 bytes (ARGB). 
            int indiceARGB = (j * largura + i) * 4;
            heightmap[j][i] = dados[indiceARGB] / 255.0f;
        }
    }

        std::vector<vertice> vertices;
    std::vector<unsigned int> indices;

    // Criar vértices do terreno
    for (int j = 0; j < altura; j++)
    {
        for (int i = 0; i < largura; i++)
        {
            vertice v;
            v.posicao = fvet3(i/(float)(largura-1), heightmap[j][i], j/(float)(altura-1));
            v.uvcoords = vetor2<float>(i / (float)largura, j / (float)altura);
        
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
    v.normal = fvet3(0.0f, 0.0f, 0.0f);
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
    fvet3 normal = fvet3(
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
    m_malha.definirVertices(vertices);
    m_malha.definirIndices(indices);
    m_malha.carregar(); // Configura VBO, VAO, EBO
}

// Construtor do terreno
terreno::terreno(const std::string &path) : diretorio(path)
{
    // Carregar imagem como Heightmap
    imageLoader imagem(path);
    largura = imagem.obterLargura();
    altura = imagem.obterAltura();
    gerarHeightMap(imagem.obterDados(), largura, altura);
}

// Método para desenhar o terreno
void terreno::desenhar()
{
    m_malha.desenhar(m_shader);
}
