/** @copyright Copyright (c) 2025 Daniel Oliveira */
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
        modelo(const char* diretorio)
        {
            if (std::filesystem::exists(diretorio))
                carregarmodelo(diretorio);
            else if (std::filesystem::exists(std::filesystem::absolute(diretorio)))
            {

                carregarmodelo(std::filesystem::absolute(diretorio).string().c_str());
            }else
            {
                depuracao::emitir(erro, "Diretorio do modelo inexistente.");
            }
        }
        void definirShader(const char* vertex,const char* frag);
        bubble::shader& shader();
        malha* obterMalha(int idx) { return &malhas[idx]; };
        void desenhar(bubble::shader& shader);
        std::vector<malha> malhas;
    private:
        // dados de malha

        std::string diretorio;
        bubble::shader* _Mshader{nullptr};

        void carregarmodelo(const std::string& path);
        void processarNo(aiNode* node, const aiScene* scene);
        malha processarMalha(aiMesh* mesh, const aiScene* scene);
        std::vector<textura> carregarMaterialETexturas(aiMaterial* mat, aiTextureType type,
            std::string typeName) const;
    };
}