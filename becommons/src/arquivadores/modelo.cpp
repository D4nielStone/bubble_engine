#include "arquivadores/modelo.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "depuracao/debug.hpp"
#include "arquivadores/imageloader.hpp"
#include <filesystem>

namespace bubble
{
    void modelo::desenhar(bubble::shader& shader)
    {
        for (unsigned int i = 0; i < malhas.size(); i++)
            malhas[i].desenhar(shader);
    }

    bubble::shader& modelo::shader()
    {
        if (!_Mshader) _Mshader = new bubble::shader();
        return *_Mshader;
    }

    void modelo::definirShader(const char* vertex, const char* frag)
    {
        if (_Mshader) delete _Mshader;
        _Mshader = new bubble::shader(vertex, frag);
    }

    void modelo::carregarmodelo(const std::string& path)
    {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            depuracao::emitir(erro, import.GetErrorString());
            return;
        }
        diretorio = path.substr(0, path.find_last_of('\\'));

        /// Processa o no principal
        processarNo(scene->mRootNode, scene);
    }

    void modelo::processarNo(aiNode* node, const aiScene* scene)
    {
        // processa todos as malhas se tiver
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* malha = scene->mMeshes[node->mMeshes[i]];
            malhas.push_back(processarMalha(malha, scene));
        }
        // then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processarNo(node->mChildren[i], scene);
        }

    }

    bubble::malha modelo::processarMalha(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<bubble::vertice> vertices;
        std::vector<unsigned int> indices;
        std::vector<textura> texturas;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            bubble::vertice vertex;
            // processa coordenadas de vertice
            bubble::vetor3<float> vector;
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
                bubble::vetor2<float> vec;

                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.uvcoords = vec;
            }
            else
                vertex.uvcoords = bubble::vetor2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        /// processa os indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        // processa materiais
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // 1. diffuse maps
        std::vector<textura> diffuseMaps = carregarMaterialETexturas(material, aiTextureType_DIFFUSE, "texture_diffuse");
        texturas.insert(texturas.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        std::vector<textura> specularMaps = carregarMaterialETexturas(material, aiTextureType_SPECULAR, "texture_specular");
        texturas.insert(texturas.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<textura> normalMaps = carregarMaterialETexturas(material, aiTextureType_HEIGHT, "texture_normal");
        texturas.insert(texturas.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<textura> heightMaps = carregarMaterialETexturas(material, aiTextureType_AMBIENT, "texture_height");
        texturas.insert(texturas.end(), heightMaps.begin(), heightMaps.end());

        /// extrai cor difusa
        aiColor4D diffuse_color;
        bubble::cor difusa;
        if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color))
        {
            difusa.r = diffuse_color.r;
            difusa.g = diffuse_color.g;
            difusa.b = diffuse_color.b;
            difusa.a = diffuse_color.a;
        }
        bubble::material mat(texturas, difusa);

        return bubble::malha(vertices, indices, mat);
    }

    std::vector<textura> modelo::carregarMaterialETexturas(aiMaterial* mat, aiTextureType type, std::string typeName) const
    {
        std::vector<textura> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
        }
        return textures;
    }
}