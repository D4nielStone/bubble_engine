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
void modelo::desenhar() {
    for(auto& malha : malhas)
        malha.desenhar(m_shader);
}

shader& modelo::obterShader() {
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
    Assimp::Importer importer;
    auto flags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
    
    // Carrega a cena sem colapsar ainda
    const aiScene* scene = importer.ReadFile(path, flags);
    if (!scene || !scene->HasMeshes() || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        // erro de carregamento
        depuracao::emitir(erro, importer.GetErrorString());
        return;
    }
    
    // Verifica se é estático
    bool is_static = true;

    // Verifica se há animações ou bones
    if (scene->HasAnimations())
        is_static = false;
    
    for (unsigned int i = 0; i < scene->mNumMeshes && is_static; ++i) {
        if (scene->mMeshes[i]->HasBones()) {
            is_static = false;
            break;
        }
    }
    if (is_static && std::filesystem::path(path).extension() != ".dae") {
        importer.FreeScene(); // limpa a cena anterior

        flags |= aiProcess_PreTransformVertices;
    
        scene = importer.ReadFile(path, flags);
        if (!scene || !scene->HasMeshes() || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            // erro de carregamento
            depuracao::emitir(erro, importer.GetErrorString());
            return;
        }
    }

    diretorio = path.substr(0, path.find_last_of('\\'));

    /// Processa o no principal
    processarNo(scene->mRootNode, scene);
    for(auto& m_ : malhas) 
        m_.carregar();
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
    aiMaterial* ai_material = scene->mMaterials[mesh->mMaterialIndex];
    becommons::material bmat;

    if(temTextura(ai_material, aiTextureType_DIFFUSE))
        bmat.definirTextura("tex_albedo", carregarTextura(ai_material, aiTextureType_DIFFUSE));
    
    if(temTextura(ai_material, aiTextureType_METALNESS))
        bmat.definirTextura("tex_metallic", carregarTextura(ai_material, aiTextureType_METALNESS));

    if(temTextura(ai_material, aiTextureType_DIFFUSE_ROUGHNESS))
        bmat.definirTextura("tex_roughness", carregarTextura(ai_material, aiTextureType_DIFFUSE_ROUGHNESS));

    if(temTextura(ai_material, aiTextureType_NORMALS))
        bmat.definirTextura("tex_normal", carregarTextura(ai_material, aiTextureType_NORMALS));

    if(temTextura(ai_material, aiTextureType_AMBIENT_OCCLUSION))
        bmat.definirTextura("tex_ao", carregarTextura(ai_material, aiTextureType_AMBIENT_OCCLUSION));

    if(temTextura(ai_material, aiTextureType_HEIGHT))
        bmat.definirTextura("tex_height", carregarTextura(ai_material, aiTextureType_HEIGHT));
    
    bmat.definirUniforme("use_tex_albedo", false);
    bmat.definirUniforme("use_tex_metallic", false);
    bmat.definirUniforme("use_tex_roughness", false);
    bmat.definirUniforme("use_tex_normal", false);
    bmat.definirUniforme("use_tex_ao", false);
    bmat.definirUniforme("use_tex_height", false);
    for (auto& [nome, tex] : bmat.texturas) {
        bmat.definirUniforme(std::string("use_") + nome, true);
    }
    /// extrai cor difusa
    aiColor4D diffuse_color;
    cor difusa;
    if (AI_SUCCESS == ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color)) {
        difusa.r = diffuse_color.r;
        difusa.g = diffuse_color.g;
        difusa.b = diffuse_color.b;
        difusa.a = diffuse_color.a;
    }
    bmat.definirUniforme("material.albedo", difusa);
    bmat.definirUniforme("material.metallic", 0.5f);
    bmat.definirUniforme("material.ao", 0.2f);
    bmat.definirUniforme("material.roughness", 0.3f);

    malha m_(vertices, indices, bmat);
    return m_;
}

// Carrega Textura assimp2bubble
textura modelo::carregarTextura(aiMaterial* mat, const aiTextureType& type) {
    textura tex;
    if (temTextura(mat, type)) {
        aiString str;
        mat->GetTexture(type, 0, &str);

        tex.path = std::filesystem::path(diretorio).parent_path().string() + "/" + std::filesystem::path(str.C_Str()).filename().string();
        tex.id = textureLoader::obterInstancia().carregarTextura(tex.path);
    }
    return tex;
}

bool modelo::temTextura(aiMaterial* mat, const aiTextureType& type) {
    return (mat->GetTextureCount(type) > 0);
}
