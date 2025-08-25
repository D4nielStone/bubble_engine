/** @copyright 
 * MIT License
 * Copyright (c) 2025 Daniel Oliveira
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 * @file shader.cpp
 */

#include "glad.h"
#include "depuracao/debug.hpp"
#include "nucleo/projeto.hpp"
#include <filesystem>
#include "assets/shaders_na_memoria.hpp"

using namespace becommons;

// Mapeia os shaders na memória para facilitar o acesso aos shaders embutidos
inline const std::unordered_map<std::string, const char*> shader_memoria{
    {"quad.frag", quad_frag},
    {"imagem.vert", imagem_vert},
    {"imagem.frag", imagem_frag},
    {"framebuffer.frag", imagem_frag},
    {"vintage.frag", vintega_frag},
    {"texto.frag", texto_frag},
    {"sombra.vs", sombra_vert},
    {"sombra.fs", sombra_frag},
    {"skybox.vs", skybox_vert},
    {"skybox.fs", skybox_frag},
    {"phong.vert", phong_vert},
    {"phong.frag", phong_frag}
};

void becommons::descarregarShaders() {
    depuracao::emitir(alerta, "shader", "Descarregando shaders.");
    for (auto& [name, programID] : shaders) {
        glDeleteProgram(programID);
    }
    shaders.clear();
}

shaderException::shaderException(const char* msg) : msg_(msg) {}

const char* shaderException::what() const noexcept {
    return msg_.c_str();
}

shader::shader(const char* vertexPath, const char* fragmentPath) {
    vert = vertexPath; frag = fragmentPath;
    compilar();
}

void shader::compilar() {
    // Verifica se o shader já foi compilado
    if(shaders.find(frag) != shaders.end())
    {
        ID = shaders[frag];
        return;
    }
    // Cria o programa shader
    try {
        ID = glCreateProgram();
    }
    catch (const std::exception& e) {
        std::cerr << "Erro ao criar shader_program: " << e.what() << std::endl;
    }

    const char* vertexshaderSource{};
    const char* fragmentshaderSource{};

    // Abre e lê os arquivos de shader
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vshaderFile, fshaderFile;

    vshaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    vshaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    if (!std::filesystem::exists(vert) || !std::filesystem::exists(frag))
    {
        // Verifica se o shader está na memória
        if (shader_memoria.find(std::filesystem::path(vert).filename().string()) != shader_memoria.end()) {
            vertexshaderSource = shader_memoria.at(std::filesystem::path(vert).filename().string());
        }
        else
            return;
        if (shader_memoria.find(std::filesystem::path(frag).filename().string()) != shader_memoria.end()) {
            fragmentshaderSource = shader_memoria.at(std::filesystem::path(frag).filename().string());
        }
        else
            return;
    }
    else
    {
        try {
            vshaderFile.open(vert);
            fshaderFile.open(frag);
            std::stringstream vshaderStream, fshaderStream;

            vshaderStream << vshaderFile.rdbuf();
            fshaderStream << fshaderFile.rdbuf();

            vertexCode = vshaderStream.str();
            fragmentCode = fshaderStream.str();
            vertexshaderSource = vertexCode.c_str();
            fragmentshaderSource = fragmentCode.c_str();
        }
        catch (const std::ifstream::failure& e)
        {
            std::cerr << e.what() << "\n";
        }
    }
    // Compilação do Malha shader
    GLuint vertexshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexshader, 1, &vertexshaderSource, NULL);
    glCompileShader(vertexshader);
    if (!checkCompileErrors(vertexshader, "VERTEX")) return;

    // Compilação do Fragment shader
    GLuint fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentshader, 1, &fragmentshaderSource, NULL);
    glCompileShader(fragmentshader);
    if (!checkCompileErrors(fragmentshader, "FRAGMENT")) return;

    // Vinculação e linkagem dos shaders
    glAttachShader(ID, vertexshader);
    glAttachShader(ID, fragmentshader);
    glLinkProgram(ID);
    if (!checkLinkErrors(ID)) return;

    // Limpeza
    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);

    
    shaders[frag] = ID;
}
shader::~shader() {
}
void shader::use() 
{
    if(shaders.find(frag) != shaders.end()) {
        glUseProgram(ID);
    }
    else {
        compilar();
    }
}

void shader::setBool(const std::string& name, const bool& value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void shader::setFloat(const std::string& name, const float& value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void shader::setInt(const std::string& name, const int& value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void shader::setMat4(const std::string& name, const float* value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}

void shader::setMat3(const std::string& name, const float* value) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}

void shader::setCor(const std::string& name, const becommons::cor& cor) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), cor.r, cor.g, cor.b, cor.a);
}

void shader::setVec4(const std::string& name, const fvet4& vec4 ) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), vec4.x, vec4.y, vec4.z, vec4.w);
}
void shader::setVec3(const std::string& name, const float& r, const float& g, const float& b) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), r, g, b);
}
void shader::setVec3(const std::string& name, const fvet3& vet) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), vet.x, vet.y, vet.z);
}

void shader::setVec2(const std::string& name, const float& r, const float& g) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), r, g);
}

bool shader::checkCompileErrors(unsigned int shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];

    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << ": " << infoLog << std::endl;
            return false;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR: " << infoLog << std::endl;
            return false;
        }
    }
    return true;
}

bool shader::checkLinkErrors(unsigned int program) {
    GLint success;
    GLchar infoLog[1024];

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        std::cerr << "PROGRAM_LINKING_ERROR: " << infoLog << std::endl;
        return false;
    }
    return true;
}
