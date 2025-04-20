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
 * @file imageloader.cpp
 */


#include "arquivadores/imageloader.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "assets/imagems_na_memoria.hpp"
#include <map>
#include <filesystem>
#include "depuracao/debug.hpp"
#include <iostream>

using namespace BECOMMONS_NS;

const std::map<const std::string, std::pair<BYTE*, const unsigned int>> imagems_memoria
{
    {"cubo_branco", std::pair(cubo_branco, cubo_branco_len)},
    {"skybox_right.png", std::pair(DaylightBox_Right, DaylightBox_Right_size)},
    {"skybox_left.png", std::pair(DaylightBox_Left, DaylightBox_Left_size)},
    {"skybox_top.png", std::pair(DaylightBox_Top, DaylightBox_Top_size)},
    {"skybox_back.png", std::pair(DaylightBox_Back, DaylightBox_Back_size)},
    {"skybox_bottom.png", std::pair(DaylightBox_Bottom, DaylightBox_Bottom_size)},
    {"skybox_front.png", std::pair(DaylightBox_Front, DaylightBox_Front_size)},
    {"icon.ico", std::pair(icon_png, icon_png_len)},
    {"banner.png", std::pair(banner_png, banner_png_len)},
    {"arco_cor.png", std::pair(arco_cor_png, arco_cor_png_len)},
    {"Camera.png", std::pair(camera_png, camera_png_len)},
    {"check.png", std::pair(check_png, check_png_len)},
    {"Codigo.png", std::pair(codigo_png, codigo_png_len)},
    {"cube.png", std::pair(cube_png, cube_png_len)},
    {"Fisica.png", std::pair(fisica_png, fisica_png_len)},
    {"Iluminacao.png", std::pair(iluminacao_png, iluminacao_png_len)},
    {"Play.png", std::pair(play_png, play_png_len)},
    {"Renderizador.png", std::pair(renderizador_png, renderizador_png_len)},
    {"scene.png", std::pair(scene_png, scene_png_len)},
    {"stop.png", std::pair(stop_png, stop_png_len)},
    {"Terreno.png", std::pair(terreno_png, terreno_png_len)},
    {"Transformacao.png", std::pair(transformacao_png, transformacao_png_len)},
    {"folder.png", std::pair(folder_png, folder_png_len)}
};
void imageLoader::shutdown()
{ FreeImage_DeInitialise(); imagens_carregadas.clear(); }
imageLoader::imageLoader()
{
}
imageLoader::imageLoader(const std::string& filepath)
    : width(0), height(0), channels(0), data(nullptr), path(filepath.c_str()), carregado(false)
{
    carregarImagem(filepath);
}
imageLoader::~imageLoader()
{
    if (data) {
        //delete[] data;
        data = nullptr; // Precau��o para evitar acesso duplo
    }
}
void imageLoader::flipVertical()
{
    int rowSize = width * channels;
    unsigned char* tempRow = new unsigned char[rowSize];
    for (int y = 0; y < height / 2; ++y) {
        unsigned char* row1 = data + y * rowSize;
        unsigned char* row2 = data + (height - 1 - y) * rowSize;
        memcpy(tempRow, row1, rowSize);
        memcpy(row1, row2, rowSize);
        memcpy(row2, tempRow, rowSize);
    }
    delete[] tempRow;
}
void imageLoader::carregarImagem(const std::string& filepath)
{
    auto it = imagens_carregadas.find(filepath);
    if (it != imagens_carregadas.end())
    {
        data = imagens_carregadas[filepath]->data;
        channels = imagens_carregadas[filepath]->channels;
        width = imagens_carregadas[filepath]->width;
        height = imagens_carregadas[filepath]->height;
        carregado = true;
        return;
    }
     depuracao::emitir(debug, "imageLoader", "nova imagem: " + filepath);

    const std::string nome_arquivo = std::filesystem::path(filepath).filename().string();
    if (imagems_memoria.find(nome_arquivo) != imagems_memoria.end())
    {
        embutida(imagems_memoria.at(nome_arquivo).first, imagems_memoria.at(nome_arquivo).second);
        return;
    }
    // Determina o formato da imagem  
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path, 0);
    if (format == FIF_UNKNOWN) {
        format = FreeImage_GetFIFFromFilename(path);
    }

    if (format == FIF_UNKNOWN) {
        return;
    }

    // Carrega a imagem  
    FIBITMAP* bitmap = FreeImage_Load(format, path);
    if (!bitmap) {
        return;
    }

    // Converte a imagem para 32 bits  
    FIBITMAP* converted = FreeImage_ConvertTo32Bits(bitmap);
    FreeImage_Unload(bitmap);

    if (!converted) {
        return;
    }

    // Obt�m as dimens�es da imagem  
    width = FreeImage_GetWidth(converted);
    height = FreeImage_GetHeight(converted);
    channels = 4;  // RGBA  

    // Aloca mem�ria para os dados da imagem  
    data = new unsigned char[width * height * channels];
    unsigned char* bits = FreeImage_GetBits(converted);

    // Copia os dados e corrige a ordem dos canais (ARGB para RGBA)  
    for (int i = 0; i < width * height; ++i) {
        data[i * 4 + 0] = bits[i * 4 + 2]; // R  
        data[i * 4 + 1] = bits[i * 4 + 1]; // G  
        data[i * 4 + 2] = bits[i * 4 + 0]; // B  
        data[i * 4 + 3] = bits[i * 4 + 3]; // A  
    }


    FreeImage_Unload(converted);

    // Indica que a imagem foi carregada com sucesso  
    carregado = true;
    flipVertical();
    imagens_carregadas[filepath] = std::make_shared<imageLoader>(*this);

}
void imageLoader::embutida(BYTE* data, const unsigned int tamanho) 
{
    // Cria um stream de mem�ria com o buffer da imagem
    FIMEMORY* memoryStream = FreeImage_OpenMemory(data, tamanho);
    if (!memoryStream) {
        fprintf(stderr, "Erro ao criar o stream de mem�ria.");
        return;
    }

    // Detecta o formato da imagem no stream de mem�ria
    FREE_IMAGE_FORMAT format = FreeImage_GetFileTypeFromMemory(memoryStream, 0);
    if (format == FIF_UNKNOWN) {
        format = FIF_PNG;
    }

    // Carrega a imagem do stream de mem�ria
    FIBITMAP* bitmap_ = FreeImage_LoadFromMemory(format, memoryStream, 0);

    if (!bitmap_) {
        fprintf(stderr, "Erro ao carregar a imagem.");
        FreeImage_CloseMemory(memoryStream);
        return;
    }
    // Converte a imagem para 32 bits  
    FIBITMAP* bitmap = FreeImage_ConvertTo32Bits(bitmap_);
    // Obt�m as propriedades da imagem (largura, altura, canais, dados)
    width = FreeImage_GetWidth(bitmap);     // Largura da imagem
    height = FreeImage_GetHeight(bitmap);   // Altura da imagem
    channels = FreeImage_GetBPP(bitmap)/8; // Canais de cor (assumindo 8 bits por canal)

    // Aloca mem�ria para os dados da imagem  
    this->data = new unsigned char[width * height * channels];
    unsigned char* bits = FreeImage_GetBits(bitmap);

    // Copia os dados e corrige a ordem dos canais (ARGB para RGBA)  
    for (int i = 0; i < width * height; ++i) {
        this->data[i * 4 + 0] = bits[i * 4 + 2]; // R  
        this->data[i * 4 + 1] = bits[i * 4 + 1]; // G  
        this->data[i * 4 + 2] = bits[i * 4 + 0]; // B  
        this->data[i * 4 + 3] = bits[i * 4 + 3]; // A  
    }

    // Libera os recursos
    FreeImage_Unload(bitmap);
    FreeImage_CloseMemory(memoryStream);

    // Indica que a imagem foi carregada com sucesso  
    carregado = true;
    flipVertical();

    return;
}
GLFWimage imageLoader::converterParaGlfw()
{
    GLFWimage image = {};
    if (!carregado) {
        return image;
    }

    image.width = width;
    image.height = height;
    image.pixels = data;
    return image;
}
int imageLoader::obterLargura() const
{
    return width;
}
int imageLoader::obterAltura() const
{
    return height;
}
int imageLoader::obterCanal() const
{
    return channels;
}
unsigned char* imageLoader::obterDados() const
{
    return data;
}

int BECOMMONS_NS::texturaDoArquivo(const std::string& directory,GLuint tipo_textura) {
    // Gera um ID de textura e carrega a imagem
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    imageLoader img(directory.c_str());
    auto data = img.obterDados();
    nrComponents = img.obterCanal();
    width = img.obterLargura();
    height = img.obterAltura(); 
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(tipo_textura, 0, GL_SRGB_ALPHA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
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
int BECOMMONS_NS::texturaDoArquivo(const std::string& directory, int* width_ptr , int* height_ptr) {
    // Gera um ID de textura e carrega a imagem
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    imageLoader img(directory.c_str());
    auto data = img.obterDados();
    nrComponents = img.obterCanal();
    width = img.obterLargura();
    height = img.obterAltura();
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
int BECOMMONS_NS::texturaDoArquivo(const std::string& directory, double* width_ptr , double* height_ptr) {
    // Gera um ID de textura e carrega a imagem
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    imageLoader img(directory.c_str());
    auto data = img.obterDados();
    nrComponents = img.obterCanal();
    width = img.obterLargura();
    height = img.obterAltura();
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
int BECOMMONS_NS::texturaDoArquivo(unsigned char* data, unsigned int width, unsigned int height, int nrComponents) {

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

textureLoader& BECOMMONS_NS::textureLoader::obterInstancia()
{
    static textureLoader instance;
    return instance;
}

GLuint textureLoader::carregarTextura(const std::string& caminho, int *width, int *height)
{
    // Verificar se a textura j� foi carregada
    if (texturasCarregadas.find(caminho) != texturasCarregadas.end()) {
        return texturasCarregadas[caminho]; // Retorna ID da textura j� carregada
    }

    // Carregar nova textura
    GLuint id = texturaDoArquivo(caminho.c_str(), width, height);
    texturasCarregadas[caminho] = id; // Armazena o ID da textura no mapa

    return id;
}
GLuint textureLoader::carregarTextura(const std::string& caminho, double *width, double *height)
{
    // Verificar se a textura j� foi carregada
    if (texturasCarregadas.find(caminho) != texturasCarregadas.end()) {
        return texturasCarregadas[caminho]; // Retorna ID da textura j� carregada
    }

    // Carregar nova textura
    GLuint id = texturaDoArquivo(caminho.c_str(), width, height);
    texturasCarregadas[caminho] = id; // Armazena o ID da textura no mapa

    return id;
}
GLuint textureLoader::carregarTextura(const std::string& caminho)
{
    // Verificar se a textura j� foi carregada
    if (texturasCarregadas.find(caminho) != texturasCarregadas.end()) {
        return texturasCarregadas[caminho]; // Retorna ID da textura j� carregada
    }

    // Carregar nova textura
    GLuint id = texturaDoArquivo(caminho.c_str(), GL_TEXTURE_2D);
    texturasCarregadas[caminho] = id; // Armazena o ID da textura no mapa

    return id;
}

GLuint textureLoader::carregarSkybox(const char* path_pai, std::vector<std::string> faces) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for (unsigned int i = 0; i < faces.size(); i++) {
        int width, height, nrChannels;
        std::string caminhoCompleto = std::string(path_pai) + "/" + faces[i];
        imageLoader img(caminhoCompleto.c_str());
        unsigned char* data = img.obterDados();
        width = img.obterLargura();
        height = img.obterAltura();
        nrChannels = img.obterCanal();

        if (img.carregado && data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        else {
            std::cerr << "Falha ao carregar a textura do Skybox: " << caminhoCompleto << std::endl;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

GLuint textureLoader::carregarAiTexture(const aiTexture* texture)
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

                // Usar GerenciadorDeTexturas para carregar a textura
                ID = texturaDoArquivo(data, width, height, numChannels);

                FreeImage_Unload(dib);
            }
            else {
            }
        }
        FreeImage_CloseMemory(fiMemory);
    }
    return ID;
}
GLuint textureLoader::carregarSkyboxMemoria(const std::vector<std::string> faces) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    // Itera sobre cada face do skybox
    for (unsigned int i = 0; i < faces.size(); i++) {
        auto it = imagems_memoria.find(faces[i]);
        if (it != imagems_memoria.end()) {
            imageLoader img;
            // Carrega a imagem diretamente da memória
            img.embutida(it->second.first, it->second.second);
            width = img.obterLargura();
            height = img.obterAltura();
            nrChannels = img.obterCanal();
        unsigned int format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
            if (img.carregado) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, GL_SRGB_ALPHA, width, height, 0, format, GL_UNSIGNED_BYTE, img.obterDados());
            } else {
                std::cerr << "Falha ao carregar a textura da skybox da memória: " << faces[i] << std::endl;
            }
        } else {
            std::cerr << "Imagem não encontrada na memória: " << faces[i] << std::endl;
        }
    }

    // Define os parâmetros da textura do cubo
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
