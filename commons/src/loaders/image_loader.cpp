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
 * @file image_loader.cpp
 */


#include "loaders/image_loader.hpp"
#include "glad.h"
#include <GLFW/glfw3.h>
#include "assets/images_in_memory.hpp"
#include <map>
#include <filesystem>
#include "debugging/debug.hpp"
#include <iostream>

using namespace COMMONS_NS;

static const std::map<const std::string, std::pair<BYTE*, const unsigned int>> imagems_memoria
{
    {"abrir.png", std::pair(abrir_png, abrir_png_len)},
    {"add.png", std::pair(add_png, add_png_len)},
    {"remove.png", std::pair(remove_png, remove_png_len)},
    {"white_cube", std::pair(white_cube, cubo_branco_len)},
    {"skybox_right.png", std::pair(DaylightBox_Right, DaylightBox_Right_size)},
    {"skybox_left.png", std::pair(DaylightBox_Left, DaylightBox_Left_size)},
    {"skybox_top.png", std::pair(DaylightBox_Top, DaylightBox_Top_size)},
    {"skybox_back.png", std::pair(DaylightBox_Back, DaylightBox_Back_size)},
    {"skybox_bottom.png", std::pair(DaylightBox_Bottom, DaylightBox_Bottom_size)},
    {"skybox_front.png", std::pair(DaylightBox_Front, DaylightBox_Front_size)},
    {"icon.ico", std::pair(icon_png, icon_png_len)},
    {"banner.png", std::pair(banner_png, banner_png_len)},
    {"info.png", std::pair(info_png, info_png_len)},
    {"color_wheel.png", std::pair(arco_cor_png, arco_cor_png_len)},
    {"Camera.png", std::pair(camera_png, camera_png_len)},
    {"check.png", std::pair(check_png, check_png_len)},
    {"Codigo.png", std::pair(codigo_png, codigo_png_len)},
    {"cube.png", std::pair(cube_png, cube_png_len)},
    {"joystick.png", std::pair(joystick_png, joystick_png_len)},
    {"Fisica.png", std::pair(fisica_png, fisica_png_len)},
    {"Iluminacao.png", std::pair(iluminacao_png, iluminacao_png_len)},
    {"Play.png", std::pair(play_png, play_png_len)},
    {"Renderizador.png", std::pair(renderizador_png, renderizador_png_len)},
    {"scene.png", std::pair(scene_png, scene_png_len)},
    {"stop.png", std::pair(stop_png, stop_png_len)},
    {"Terreno.png", std::pair(terreno_png, terreno_png_len)},
    {"Transformacao.png", std::pair(transform_png, transform_png_len)},
    {"folder.png", std::pair(folder_png, folder_png_len)}
};
void image_loader::shutdown()
{ FreeImage_DeInitialise(); imagens_carregadas.clear(); }
image_loader::image_loader()
{
}
image_loader::image_loader(const std::string& filepath)
    : width(0), height(0), channels(0), data(nullptr), path(filepath.c_str()), loaded(false)
{
    load_image(filepath);
}
image_loader::~image_loader()
{
    if (data) {
        //delete[] data;
        data = nullptr; // Precau��o para evitar acesso duplo
    }
}
void image_loader::flipVertical()
{
    int rowSize = width * channels;
    unsigned char* haspRow = new unsigned char[rowSize];
    for (int y = 0; y < height / 2; ++y) {
        unsigned char* row1 = data + y * rowSize;
        unsigned char* row2 = data + (height - 1 - y) * rowSize;
        memcpy(haspRow, row1, rowSize);
        memcpy(row1, row2, rowSize);
        memcpy(row2, haspRow, rowSize);
    }
    delete[] haspRow;
}
void image_loader::load_image(const std::string& filepath)
{
    auto it = imagens_carregadas.find(filepath);
    if (it != imagens_carregadas.end())
    {
        data = imagens_carregadas[filepath]->data;
        channels = imagens_carregadas[filepath]->channels;
        width = imagens_carregadas[filepath]->width;
        height = imagens_carregadas[filepath]->height;
        loaded = true;
        return;
    }
     debugging::emit(debug, "image_loader", "new image: " + filepath);

    const std::string file_name = std::filesystem::path(filepath).filename().string();
    if (imagems_memoria.find(file_name) != imagems_memoria.end())
    {
        embutida(imagems_memoria.at(file_name).first, imagems_memoria.at(file_name).second);
        return;
    }
    // Determina o formato da image
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path, 0);
    if (format == FIF_UNKNOWN) {
        format = FreeImage_GetFIFFromFilename(path);
    }

    if (format == FIF_UNKNOWN) {
        return;
    }

    // Carrega a image
    FIBITMAP* bitmap = FreeImage_Load(format, path);
    if (!bitmap) {
        return;
    }

    // Converte a image para 32 bits
    FIBITMAP* converted = FreeImage_ConvertTo32Bits(bitmap);
    FreeImage_Unload(bitmap);

    if (!converted) {
        return;
    }

    // Obt�m as dimens�es da image
    width = FreeImage_GetWidth(converted);
    height = FreeImage_GetHeight(converted);
    channels = 4;  // RGBA

    // Aloca mem�ria para os data da image
    data = new unsigned char[width * height * channels];
    unsigned char* bits = FreeImage_GetBits(converted);

    // Copia os data e corrige a ordem dos canais (ARGB para RGBA)
    for (int i = 0; i < width * height; ++i) {
        data[i * 4 + 0] = bits[i * 4 + 2]; // R
        data[i * 4 + 1] = bits[i * 4 + 1]; // G
        data[i * 4 + 2] = bits[i * 4 + 0]; // B
        data[i * 4 + 3] = bits[i * 4 + 3]; // A
    }


    FreeImage_Unload(converted);

    // Indica que a image foi carregada com sucesso
    loaded = true;
    flipVertical();
    imagens_carregadas[filepath] = std::make_shared<image_loader>(*this);

}
void image_loader::embutida(BYTE* data, const unsigned int size)
{
    // Cria um stream de mem�ria com o buffer da image
    FIMEMORY* memoryStream = FreeImage_OpenMemory(data, size);
    if (!memoryStream) {
        fprintf(stderr, "Erro ao create o stream de mem�ria.");
        return;
    }

    // Detecta o formato da image no stream de mem�ria
    FREE_IMAGE_FORMAT format = FreeImage_GetFileTypeFromMemory(memoryStream, 0);
    if (format == FIF_UNKNOWN) {
        format = FIF_PNG;
    }

    // Carrega a image do stream de mem�ria
    FIBITMAP* bitmap_ = FreeImage_LoadFromMemory(format, memoryStream, 0);

    if (!bitmap_) {
        fprintf(stderr, "Erro ao load a image.");
        FreeImage_CloseMemory(memoryStream);
        return;
    }
    // Converte a image para 32 bits
    FIBITMAP* bitmap = FreeImage_ConvertTo32Bits(bitmap_);
    // Obt�m as properties da image (width, height, canais, data)
    width = FreeImage_GetWidth(bitmap);     // Largura da image
    height = FreeImage_GetHeight(bitmap);   // Altura da image
    channels = FreeImage_GetBPP(bitmap)/8; // Canais de color (assumindo 8 bits por channel)

    // Aloca mem�ria para os data da image
    this->data = new unsigned char[width * height * channels];
    unsigned char* bits = FreeImage_GetBits(bitmap);

    // Copia os data e corrige a ordem dos canais (ARGB para RGBA)
    for (int i = 0; i < width * height; ++i) {
        this->data[i * 4 + 0] = bits[i * 4 + 2]; // R
        this->data[i * 4 + 1] = bits[i * 4 + 1]; // G
        this->data[i * 4 + 2] = bits[i * 4 + 0]; // B
        this->data[i * 4 + 3] = bits[i * 4 + 3]; // A
    }

    // Libera os recursos
    FreeImage_Unload(bitmap);
    FreeImage_CloseMemory(memoryStream);

    // Indica que a image foi carregada com sucesso
    loaded = true;
    flipVertical();

    return;
}
GLFWimage image_loader::convert_to_glfw()
{
    GLFWimage image = {};
    if (!loaded) {
        return image;
    }

    image.width = width;
    image.height = height;
    image.pixels = data;
    return image;
}
int image_loader::get_width() const
{
    return width;
}
int image_loader::get_height() const
{
    return height;
}
int image_loader::getCanal() const
{
    return channels;
}
unsigned char* image_loader::getDados() const
{
    return data;
}

int COMMONS_NS::texture_from_file(const std::string& directory,GLuint texture_type) {
    // Gera um ID de texture e carrega a image
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    image_loader img(directory.c_str());
    auto data = img.getDados();
    nrComponents = img.getCanal();
    width = img.get_width();
    height = img.get_height();
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(texture_type, 0, GL_SRGB_ALPHA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    }
    else {
        std::cerr << "Failed to load texture: " << directory << std::endl;
        return -1;
    }

    return textureID;
}
int COMMONS_NS::texture_from_file(const std::string& directory, int* width_ptr , int* height_ptr) {
    // Gera um ID de texture e carrega a image
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    image_loader img(directory.c_str());
    auto data = img.getDados();
    nrComponents = img.getCanal();
    width = img.get_width();
    height = img.get_height();
    if (width_ptr) *width_ptr = width;
    if (height_ptr) *height_ptr = height;
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    }
    else {
        std::cerr << "Failed to load texture: " << directory << std::endl;
        return -1;
    }

    return textureID;
}
int COMMONS_NS::texture_from_file(const std::string& directory, double* width_ptr , double* height_ptr) {
    // Gera um ID de texture e carrega a image
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    image_loader img(directory.c_str());
    auto data = img.getDados();
    nrComponents = img.getCanal();
    width = img.get_width();
    height = img.get_height();
    if (width_ptr) *width_ptr = width;
    if (height_ptr) *height_ptr = height;
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    }
    else {
        std::cerr << "Failed to load texture: " << directory << std::endl;
        return -1;
    }

    return textureID;
}
int COMMONS_NS::texture_from_file(unsigned char* data, unsigned int width, unsigned int height, int nrComponents) {

    unsigned int textureID;
    glGenTextures(1, &textureID);

    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, 500, 500, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    }
    else {
        std::cerr << "Failed to load texture: Textura embutida" << std::endl;
        return -1;
    }

    return textureID;
}

texture_loader& COMMONS_NS::texture_loader::get_instance()
{
    static texture_loader instance;
    return instance;
}

GLuint texture_loader::load_texture(const std::string& path, int *width, int *height)
{
    // Verificar se a texture j� foi carregada
    if (loaded_textures.find(path) != loaded_textures.end()) {
        return loaded_textures[path]; // Retorna ID da texture j� carregada
    }

    // Carregar nova texture
    GLuint id = texture_from_file(path.c_str(), width, height);
    loaded_textures[path] = id; // Armazena o ID da texture no mapa

    return id;
}

GLuint texture_loader::load_texture(const std::string& path, ivector_type2& vector_type2) {
    // Verificar se a texture já foi carregada
    if (loaded_textures.find(path) != loaded_textures.end()) {
        return loaded_textures[path]; // Retorna ID da texture j� carregada
    }

    int width, height;
    // Carregar nova texture
    GLuint id = texture_from_file(path.c_str(), &width, &height);
    loaded_textures[path] = id; // Armazena o ID da texture no mapa

    vector_type2.x = width;
    vector_type2.y = height;

    return id;
}

GLuint texture_loader::load_texture(const std::string& path, fvector_type2& vector_type2) {
    // Verificar se a texture já foi carregada
    if (loaded_textures.find(path) != loaded_textures.end()) {
        return loaded_textures[path]; // Retorna ID da texture j� carregada
    }

    int width, height;
    // Carregar nova texture
    GLuint id = texture_from_file(path.c_str(), &width, &height);
    loaded_textures[path] = id; // Armazena o ID da texture no mapa

    vector_type2.x = width;
    vector_type2.y = height;

    return id;
}

GLuint texture_loader::load_texture(const std::string& path, double *width, double *height)
{
    // Verificar se a texture j� foi carregada
    if (loaded_textures.find(path) != loaded_textures.end()) {
        return loaded_textures[path]; // Retorna ID da texture j� carregada
    }

    // Carregar nova texture
    GLuint id = texture_from_file(path.c_str(), width, height);
    loaded_textures[path] = id; // Armazena o ID da texture no mapa

    return id;
}
GLuint texture_loader::load_texture(const std::string& path)
{
    // Verificar se a texture j� foi carregada
    if (loaded_textures.find(path) != loaded_textures.end()) {
        return loaded_textures[path]; // Retorna ID da texture j� carregada
    }

    // Carregar nova texture
    GLuint id = texture_from_file(path.c_str(), GL_TEXTURE_2D);
    loaded_textures[path] = id; // Armazena o ID da texture no mapa

    return id;
}

GLuint texture_loader::loadSkybox(const char* path_pai, std::vector<std::string> faces) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for (unsigned int i = 0; i < faces.size(); i++) {
        int width, height, nrChannels;
        std::string full_path = std::string(path_pai) + "/" + faces[i];
        image_loader img(full_path.c_str());
        unsigned char* data = img.getDados();
        width = img.get_width();
        height = img.get_height();
        nrChannels = img.getCanal();

        if (img.loaded && data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        else {
            std::cerr << "Failure ao load a texture do Skybox: " << full_path << std::endl;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

GLuint texture_loader::loadAiTexture(const aiTexture* texture)
{
    GLuint ID{};
    if (texture) {
        FIMEMORY* fiMemory = FreeImage_OpenMemory(reinterpret_cast<BYTE*>(texture->pcData), texture->mWidth);
        FREE_IMAGE_FORMAT format = FreeImage_GetFileTypeFromMemory(fiMemory);

        if (format != FIF_UNKNOWN) {
            FIBITMAP* dib = FreeImage_LoadFromMemory(format, fiMemory);
            if (dib) {
                BYTE* data = FreeImage_GetBits(dib);
                unsigned int width = FreeImage_GetWidth(dib);
                unsigned int height = FreeImage_GetHeight(dib);
                unsigned int bpp = FreeImage_GetBPP(dib);
                int numChannels = bpp / 8;

                // Usar GerenciadorDeTexturas para load a texture
                ID = texture_from_file(data, width, height, numChannels);

                FreeImage_Unload(dib);
            }
            else {
            }
        }
        FreeImage_CloseMemory(fiMemory);
    }
    return ID;
}
GLuint texture_loader::load_skybox_from_memory(const std::vector<std::string> faces) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    // Itera sobre cada face do skybox
    for (unsigned int i = 0; i < faces.size(); i++) {
        auto it = imagems_memoria.find(faces[i]);
        if (it != imagems_memoria.end()) {
            image_loader img;
            // Carrega a image diretamente da memória
            img.embutida(it->second.first, it->second.second);
            width = img.get_width();
            height = img.get_height();
            nrChannels = img.getCanal();
        unsigned int format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
            if (img.loaded) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, GL_SRGB_ALPHA, width, height, 0, format, GL_UNSIGNED_BYTE, img.getDados());
            } else {
                std::cerr << "Failure ao load a texture da skybox da memória: " << faces[i] << std::endl;
            }
        } else {
            std::cerr << "Imagem não encontrada na memória: " << faces[i] << std::endl;
        }
    }

    // Define os parameters da texture do cube
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
