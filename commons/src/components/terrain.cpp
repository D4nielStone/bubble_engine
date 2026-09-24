#include "components/terrain.hpp"
#include "loaders/image_loader.hpp"
#include "debugging/debug.hpp"
#include "util/vertex.hpp"
#include "util/vector3.hpp"
#include <iostream>

using namespace COMMONS_NS;

bool terrain::analyze(const rapidjson::Value& value) {
    if(value.HasMember("heightmap") && value["heightmap"].IsString())
        directory = value["heightmap"].GetString();
    // Carregar image como Heightmap
    image_loader image(directory);
    width = image.get_width();
    height = image.get_height();
    gerarHeightMap(image.getDados(), width, height);
    return true;
}

bool terrain::serialize(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const {
    value.AddMember("heightmap", rapidjson::Value(directory.c_str(), allocator), allocator);
    return true;
}

void terrain::gerarHeightMap(unsigned char* data, int width, int height)
{
    heightmap = std::vector<std::vector<float>>(height, std::vector<float>(width));
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            // Cada pixel has 4 bytes (ARGB).
            int indiceARGB = (j * width + i) * 4;
            heightmap[j][i] = data[indiceARGB] / 255.0f;
        }
    }

        std::vector<vertex> vertices;
    std::vector<unsigned int> indices;

    // Criar vértices do terrain
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            vertex v;
            v.position = fvector_type3(i/(float)(width-1), heightmap[j][i], j/(float)(height-1));
            v.uvcoords = vector2<float>(i / (float)width, j / (float)height);

            vertices.push_back(v);
        }
    }

    // Criar índices para triângulos
    for (int j = 0; j < height - 1; j++)
    {
        for (int i = 0; i < width - 1; i++)
        {
            int topLeft = j * width + i;
            int topRight = topLeft + 1;
            int bottomLeft = (j + 1) * width + i;
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
    v.normal = fvector_type3(0.0f, 0.0f, 0.0f);
    }

    // Percorrer os triângulos e calculate as normais das faces
    for (size_t i = 0; i < indices.size(); i += 3)
    {
    int i1 = indices[i];
    int i2 = indices[i + 1];
    int i3 = indices[i + 2];

    glm::vec3 v1 = {vertices[i1].position.x,vertices[i1].position.y,vertices[i1].position.z};
    glm::vec3 v2 = {vertices[i2].position.x,vertices[i2].position.y,vertices[i2].position.z};
    glm::vec3 v3 = {vertices[i3].position.x,vertices[i3].position.y,vertices[i3].position.z};

    glm::vec3 _normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
    fvector_type3 normal = fvector_type3(
        _normal.x,
        _normal.y,
        _normal.z
    );

    // Adicionar a normal ao vértice (antes de normalize no final)
    vertices[i1].normal += normal;
    vertices[i2].normal += normal;
    vertices[i3].normal += normal;
    }

    // Normalizar todas as normais dos vértices
    for (auto &v : vertices)
    {
        v.normal.normalize();
    }
    // Criar a mesh
    m_mesh.set_vertices(vertices);
    m_mesh.set_indices(indices);
    m_mesh.load(); // Configura VBO, VAO, EBO
}

// Constructor do terrain
terrain::terrain(const std::string &path) : directory(path)
{
    // Carregar image como Heightmap
    image_loader image(path);
    width = image.get_width();
    height = image.get_height();
    gerarHeightMap(image.getDados(), width, height);
}

// Método para draw o terrain
void terrain::draw()
{
    m_mesh.draw(m_shader);
}
