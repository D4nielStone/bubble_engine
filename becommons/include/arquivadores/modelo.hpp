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
 * @file modelo.hpp
 * @brief Carrega um modelo usando a biblioteca assimp com base em um diretório.
 *
 * @see modelo.cpp
 */

#pragma once
#include <assimp/scene.h>
#include <filesystem>
#include "becommons_namespace.hpp"
#include "util/malha.hpp"
#include <iostream>
#include "shader.hpp"

namespace BECOMMONS_NS {
    class modelo {
    public:
        modelo() = default;
        modelo(const char* diretorio); 
        modelo(const std::string& diretorio); 

        malha& obterMalha(size_t i);
        shader& obterShader();
        void definirShader(const shader&);
        void desenhar();
        void carregarModelo(const std::string& path);
        std::string obterDiretorio() const;
        std::vector<malha> malhas;

        void processarNo(aiNode* node, const aiScene* scene);
        malha processarMalha(aiMesh* mesh, const aiScene* scene);
        textura carregarTextura(aiMaterial*, const aiTextureType&);
        static bool temTextura(aiMaterial*, const aiTextureType&);
    protected:
        std::string diretorio;
        shader m_shader;
    };
}
