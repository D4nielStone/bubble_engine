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
 * @file imageloader.hpp
 *
 * @see imageloader.cpp
 */

#pragma once
#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include <FreeImage.h>
#include <assimp/scene.h>
#include <vector>
#include <memory>
#include "namespace.hpp"

struct GLFWimage;

namespace BECOMMONS_NS {
    // @class imageLoader
    // Gerencia as imagems/texturas carregadas na engine
    class  imageLoader {
    public:
        // Inicializa/Desliga Biblioteca FreeImage
        // @{
        static void init() { FreeImage_Initialise(); }
        static void shutdown(); 
        // @}
        // @name Construtores/Destrutores
        // @{
        imageLoader();
        imageLoader(const std::string& filepath);
        ~imageLoader();
        // @}
        // Obtenção da instância global
        static imageLoader& obterInstancia();
        // @name Obtenções de dados de imagem
        // @{
        int obterLargura() const;
        int obterAltura() const;
        int obterCanal() const;
        unsigned char* obterDados() const;
        GLFWimage converterParaGlfw();
        bool carregado;
        // @}
        void flipVertical();
        void carregarImagem(const std::string& filepath);
        void embutida(BYTE* data, const unsigned int tamanho);
    private:
        const char* path;
        int width, height, channels;
        unsigned char* data;
    };
    int texturaDoArquivo(const std::string& directory, int* width, int* height);
    int texturaDoArquivo(const std::string& directory, double* width = nullptr, double* height = nullptr);
    int texturaDoArquivo(const std::string& directory, GLuint tipo_textura);
    int texturaDoArquivo(unsigned char* data, unsigned int width, unsigned int height, int format);
    inline std::unordered_map<std::string, std::shared_ptr<imageLoader>> imagens_carregadas;
    class  textureLoader
    {
    public:
        static textureLoader& obterInstancia();
        GLuint carregarTextura(const std::string& caminho, int* width, int* height);
        GLuint carregarTextura(const std::string& caminho, double* width, double* height);
        GLuint carregarTextura(const std::string& caminho);
        GLuint carregarSkybox(const char*, std::vector<std::string> faces);
        GLuint carregarSkyboxMemoria(const std::vector<std::string> faces = 
                {
"skybox_right.png",
"skybox_left.png", 
"skybox_top.png",
"skybox_bottom.png",
"skybox_front.png",                
"skybox_back.png", 
                }); 
        GLuint carregarAiTexture(const aiTexture* texture);
        std::unordered_map<std::string, GLuint> texturasCarregadas;

    private:
        // Construtor privado para Singleton
        textureLoader() {}

        // Desabilitar cópia e atribuição
        textureLoader(const textureLoader&) = delete;
        void operator=(const textureLoader&) = delete;
    };
}
