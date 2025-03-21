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
        std::unordered_map<std::string, textura> texturas;

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
                vet2 vec;

                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.uvcoords = vec;
            }
            else
                vertex.uvcoords = vet2(0.0f, 0.0f);

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

    // Carrega Textura assimp2bubble
    textura modelo::carregarTextura(aiMaterial* mat, aiTextureType type) const
    {
        textura tex;
        if (mat->GetTextureCount(type) > 0)
        {
            aiString str;
            mat->GetTexture(type, 0, &str);

            tex.path = std::filesystem::path(diretorio).parent_path().string() + "/" + std::string(str.C_Str());
            tex.id = textureLoader::obterInstancia().carregarTextura(tex.path);
        }

        return tex;
    }
}
