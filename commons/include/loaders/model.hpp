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
 * @file model.hpp
 * @brief Carrega um model usando a biblioteca assimp com base em um diretório.
 *
 * @see model.cpp
 */

#pragma once
#include <assimp/scene.h>
#include <filesystem>
#include "commons_namespace.hpp"
#include "util/mesh.hpp"
#include <iostream>
#include "shader.hpp"

namespace COMMONS_NS {
    class model {
    public:
        model() = default;
        model(const char* directory);
        model(const std::string& directory);

        mesh& getMalha(size_t i);
        shader& getShader();
        void setShader(const shader&);
        void draw();
        void load_model(const std::string& path);
        std::string getDiretorio() const;
        std::vector<mesh> meshes;

        void process_node(aiNode* node, const aiScene* scene);
        mesh process_mesh(aiMesh* mesh, const aiScene* scene);
        texture load_texture(aiMaterial*, const aiTextureType&);
        static bool hasTextura(aiMaterial*, const aiTextureType&);
    protected:
        std::string directory;
        shader m_shader;
    };
}
