/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
#include <string>
#include "unordered_map"
#include <FreeImage.h>
#include <assimp/scene.h>
#include <vector>
#include <glad/glad.h>

struct GLFWimage;

namespace bubble
{
    class  imageLoader
    {
    public:
        imageLoader();
        imageLoader(const std::string& filepath);
        ~imageLoader();

        static bubble::imageLoader& obterInstancia();
        int obterLargura() const;
        int obterAltura() const;
        int obterCanal() const;
        unsigned char* obterDados() const;
        GLFWimage converterParaGlfw();
        bool carregado;
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

        // Desabilitar c�pia e atribui��o
        textureLoader(const textureLoader&) = delete;
        void operator=(const textureLoader&) = delete;
    };
}
