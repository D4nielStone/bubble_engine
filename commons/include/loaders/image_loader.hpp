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
 * @file image_loader.hpp
 */

#pragma once
#include "glad.h"
#include <GLFW/glfw3.h>
#include <string>
#include <unordered_map>
#include <FreeImage.h>
#include <assimp/scene.h>
#include <vector>
#include <memory>
#include "commons_namespace.hpp"
#include "util/vector2.hpp"

/// namespace commons
namespace COMMONS_NS {
    /// @class image_loader
    /// Gerencia as images/textures loaded na engine
    class  image_loader {
    public:
        /// Inicializa/Desliga Biblioteca FreeImage
        /// @{
        /// Inicializa a biblioteca
        static void init() { FreeImage_Initialise(); }
        /// Deslica a biblioteca
        static void shutdown();
        /// @}
        /// @name Constructores/Destrutores
        /// @{
        image_loader();
        /// Carrega uma image pelo diretório
        /// @param filepath Diretório do file
        image_loader(const std::string& filepath);
        /// Destrutor default
        ~image_loader();
        /// @}
        /// Obtenção da instância global
        static image_loader& get_instance();
        /// @name Obtenções de data de image
        /// @{
        int get_width() const;                   ///< @returns width da image carregada
        int get_height() const;                    ///< @returns height da image carregada
        int getCanal() const;                     ///< @returns channel (Ex: RGB/RGBA) da image carregada
        unsigned char* getDados() const;          ///< @returns Dados binários da image carregada
        GLFWimage convert_to_glfw();              ///< @returns Versão do glfw da image carregada
        bool loaded;                             ///< Flag de image carregada
        /// @}
        /// Efetua o flip vertical bit por bit em data
        void flipVertical();
        /// Carrega image pelo diretório
        /// @param filepath Diretório
        void load_image(const std::string& filepath);
        /// Carrega image embutida num array
        /// Ex: const unsigned int data[xxx] {...}
        void embutida(BYTE* data, const unsigned int size);
    private:
        const char* path;   ///< Diretório da image
        int width,          ///< Largura
            height,         ///< Altura
            channels;       ///< Canal
        unsigned char* data;///< Dados Binários
    };
    /// @name Carrega Textura do file
    /// @note Não apenas carrega a image como o image_loader,
    /// mas também cria a texture opengl e armazena num map.
    /// @{
    int texture_from_file(const std::string& directory, int* width, int* height);
    int texture_from_file(const std::string& directory, double* width = nullptr, double* height = nullptr);
    int texture_from_file(const std::string& directory, GLuint texture_type);
    int texture_from_file(unsigned char* data, unsigned int width, unsigned int height, int format);
    inline std::unordered_map<std::string, std::shared_ptr<image_loader>> imagens_carregadas;
    /// @}
    /// @class Texture loader
    class  texture_loader
    {
    public:
        static texture_loader& get_instance();
        GLuint load_texture(const std::string&, fvector_type2&);
        GLuint load_texture(const std::string&, ivec2&);
        GLuint load_texture(const std::string& path, int* width, int* height);
        GLuint load_texture(const std::string& path, double* width, double* height);
        GLuint load_texture(const std::string& path);
        GLuint loadSkybox(const char*, std::vector<std::string> faces);
        GLuint load_skybox_from_memory(const std::vector<std::string> faces =
                {
"skybox_right.png",
"skybox_left.png",
"skybox_top.png",
"skybox_bottom.png",
"skybox_front.png",
"skybox_back.png",
                });
        GLuint loadAiTexture(const aiTexture* texture);
        std::unordered_map<std::string, GLuint> loaded_textures;

    private:
        /// Constructor privado para Singleton
        texture_loader() {}

        /// Desabilitar cópia e atribuição
        texture_loader(const texture_loader&) = delete;
        void operator=(const texture_loader&) = delete;
    };
}
/// @see image_loader.cpp
