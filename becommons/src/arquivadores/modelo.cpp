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
 * @file modelo.cpp
 */

#include "arquivadores/modelo.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>
#include <map>
#include "depuracao/debug.hpp"
#include "arquivadores/imageloader.hpp"
#include "assets/objetos/cubo.hpp"
#include "assets/objetos/esfera.hpp"
#include "util/material.hpp"

using namespace BECOMMONS_NS;

std::map<std::string, malha> primitivas =  { {"cubo", malha_cubo}, {"esfera", malha_esfera}
};

modelo::modelo(const char* diretorio) {
    carregarModelo(std::filesystem::absolute(diretorio).string().c_str());
}
modelo::modelo(const std::string& diretorio) {
    carregarModelo(std::filesystem::absolute(diretorio).string().c_str());
}
malha& modelo::obterMalha(size_t i) {
    if(i < malhas.size()) {
        return malhas[i];
    }
    return malhas.back();
}
void modelo::desenhar(shader& shader) {
    for(auto& malha : malhas)
        malha.desenhar(shader);
}

shader modelo::obterShader() const {
    return m_shader;
}

void modelo::definirShader(const shader& s) {
    m_shader = s;
}
        
std::string modelo::obterDiretorio() const {
    return diretorio;
}

void modelo::carregarModelo(const std::string& path) {
    malhas.clear();
    if(primitivas.find(std::filesystem::path(path).filename().string()) != primitivas.end()) {
        diretorio = path;
        malhas.push_back(primitivas[std::filesystem::path(path).filename().string()]);
        malhas.back().carregar();
        return;
    }
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        depuracao::emitir(erro, import.GetErrorString());
        return;
    }
    diretorio = path.substr(0, path.find_last_of('\\'));

    /// Processa o no principal
    processarNo(scene->mRootNode, scene);
}

void modelo::processarNo(aiNode* node, const aiScene* scene) {
    // processa todos as malhas se tiver
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* malha = scene->mMeshes[node->mMeshes[i]];
        malhas.push_back(processarMalha(malha, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processarNo(node->mChildren[i], scene);
    }

}

malha modelo::processarMalha(aiMesh* mesh, const aiScene* scene) {
    std::vector<vertice> vertices;
    std::vector<unsigned int> indices;
    std::unordered_map<std::string, textura> texturas;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        vertice vertex;
        // processa coordenadas de vertice
        vetor3<float> vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.posicao = vector;

        // normais
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }

        // uvs
        if (mesh->mTextureCoords[0])
        {
            vetor2<float> vec;

            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.uvcoords = vec;
        }
        else
            vertex.uvcoords = vetor2<float>(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    /// processa os indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // processa materiais
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // 1. diffusa
    texturas["tex_albedo"] = carregarTextura(material, aiTextureType_DIFFUSE);
    // 2. metallico
    texturas["tex_metallic"] = carregarTextura(material, aiTextureType_METALNESS);
    // 3. roughness
    texturas["tex_roughness"] = carregarTextura(material, aiTextureType_DIFFUSE_ROUGHNESS);
    // 4. normal
    texturas["tex_normal"] = carregarTextura(material, aiTextureType_NORMALS);
    // 5. ao
    texturas["tex_ao"] = carregarTextura(material, aiTextureType_AMBIENT_OCCLUSION);
    // 6. height
    texturas["tex_height"] = carregarTextura(material, aiTextureType_HEIGHT);
    
    /// extrai cor difusa
    aiColor4D diffuse_color;
    cor difusa;
    if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color)) {
        difusa.r = diffuse_color.r;
        difusa.g = diffuse_color.g;
        difusa.b = diffuse_color.b;
        difusa.a = diffuse_color.a;
    }
    BECOMMONS_NS::material mat(texturas, difusa);

    malha m_(vertices, indices, mat);
    m_.carregar();
    return m_;
}

// Carrega Textura assimp2bubble
textura modelo::carregarTextura(aiMaterial* mat, aiTextureType type) const {
    textura tex;
    if (mat->GetTextureCount(type) > 0) {
        aiString str;
        mat->GetTexture(type, 0, &str);

        tex.path = std::filesystem::path(diretorio).parent_path().string() + "/" + std::string(str.C_Str());
        tex.id = textureLoader::obterInstancia().carregarTextura(tex.path);
    }

    return tex;
}
