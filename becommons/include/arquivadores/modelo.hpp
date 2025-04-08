/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file modelo.hpp
 * @brief Carrega um modelo usando a biblioteca assimp com base em um diretório.
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 *
 * @see modelo.cpp
 */

#pragma once
#include "util/malha.hpp"
#include "depuracao/debug.hpp"
#include "util/textura.hpp"
#include "arquivadores/shader.hpp"
#include <assimp/scene.h>
#include <filesystem>

namespace bubble
{
    class modelo
    {
    public:
        modelo() = default;
        modelo(const char* diretorio)
        {
                carregarmodelo(std::filesystem::absolute(diretorio).string().c_str());
        }

        malha* obterMalha(const int i)
        {
            if(i < malhas.size()) {
                return &malhas[i];
            }
            return nullptr;
        }
        void definirShader(const char* vertex,const char* frag);
        bubble::shader& shader();
        void desenhar(bubble::shader& shader);
        std::vector<malha> malhas;
        std::string diretorio;
    protected:
        // dados de malha

        bubble::shader* _Mshader{nullptr};

        void carregarmodelo(const std::string& path);
        void processarNo(aiNode* node, const aiScene* scene);
        malha processarMalha(aiMesh* mesh, const aiScene* scene);
        textura carregarTextura(aiMaterial* mat, aiTextureType type) const;
    };
}
