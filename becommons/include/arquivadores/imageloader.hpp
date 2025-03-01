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
    private:
        void flipVertical();
        void carregarImagem(const std::string& filepath);
        void embutida(BYTE* data, const unsigned int tamanho);
        const char* path;
        int width, height, channels;
        unsigned char* data;
    };
    int texturaDoArquivo(const std::string& directory, int* width = nullptr, int* height = nullptr);
    int texturaDoArquivo(const std::string& directory, double* width = nullptr, double* height = nullptr);
    int texturaDoArquivo(const std::string& directory, GLuint tipo_textura);
    int texturaDoArquivo(unsigned char* data, unsigned int width, unsigned int height, int format);
    class  textureLoader
    {
    public:
        static textureLoader& obterInstancia();
        GLuint carregarTextura(const std::string& caminho, int* width = nullptr, int* height = nullptr);
        GLuint carregarTextura(const std::string& caminho, double* width, double* height);
        GLuint carregarTextura(const std::string& caminho, GLuint tipo_textura = GL_TEXTURE_2D);
        GLuint carregarSkybox(const char*, std::vector<std::string> faces);
        GLuint carregarAiTexture(const aiTexture* texture);
    private:
        std::unordered_map<std::string, GLuint> texturasCarregadas;

        // Construtor privado para Singleton
        textureLoader() {}

        // Desabilitar c�pia e atribui��o
        textureLoader(const textureLoader&) = delete;
        void operator=(const textureLoader&) = delete;
    };
}