/** @copyright Copyright (c) 2025 Daniel Oliveira */
#include <glad/glad.h>
#include "arquivadores/shader.hpp"
#include <filesystem>
#include "assets/shaders_na_memoria.hpp"

inline std::vector<std::pair<std::pair<const char*, const char*>, unsigned int>> shaders;
// Mapeia os shaders na memória para facilitar o acesso aos shaders embutidos
inline const std::unordered_map<std::string, const char*> shader_memoria{
    {"quad.vert", quad_vert},
    {"quad.frag", quad_frag},
    {"imagem.vert", imagem_vert},
    {"imagem.frag", imagem_frag},
    {"texto.vert", texto_vert},
    {"texto.frag", texto_frag},
    {"skybox.vert", skybox_vert},
    {"skybox.frag", skybox_frag},
    {"phong.vert", phong_vert},
    {"phong.frag", phong_frag}
};

bubble::shaderException::shaderException(const char* msg) : msg_(msg) {}

const char* bubble::shaderException::what() const noexcept {
    return msg_.c_str();
}

bubble::shader::shader(const char* vertexPath, const char* fragmentPath) {
    compilar(vertexPath, fragmentPath);
}

void bubble::shader::compilar(const char* vertexPath, const char* fragmentPath) {
    // Verifica se o shader já foi compilado
    for (const auto& shader : shaders) {
        if (shader.first.first == vertexPath && shader.first.second == fragmentPath) {
            ID = shader.second;
            return;
        }
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

    if (!std::filesystem::exists(vertexPath) || !std::filesystem::exists(fragmentPath))
    {
        // Verifica se o shader está na memória
        if (shader_memoria.find(std::filesystem::path(vertexPath).filename().string()) != shader_memoria.end()) {
            vertexshaderSource = shader_memoria.at(std::filesystem::path(vertexPath).filename().string());
        }
        else
            return;
        if (shader_memoria.find(std::filesystem::path(fragmentPath).filename().string()) != shader_memoria.end()) {
            fragmentshaderSource = shader_memoria.at(std::filesystem::path(fragmentPath).filename().string());
        }
        else
            return;
    }
    else
    {
        try {
            vshaderFile.open(vertexPath);
            fshaderFile.open(fragmentPath);
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

    
    shaders.push_back({ {vertexPath, fragmentPath}, ID });
}

void bubble::shader::use() const {
    glUseProgram(ID);
}

void bubble::shader::setBool(const std::string& name, const bool& value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void bubble::shader::setFloat(const std::string& name, const float& value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void bubble::shader::setInt(const std::string& name, const int& value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void bubble::shader::setMat4(const std::string& name, const float* value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}

void bubble::shader::setMat3(const std::string& name, const float* value) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}

void bubble::shader::setCor(const std::string& name, const bubble::cor& cor) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), cor.r, cor.g, cor.b, cor.a);
}

void bubble::shader::setVec4(const std::string& name, const vetor4<int>& vec4 ) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), vec4.x, vec4.y, vec4.w, vec4.h);
}
void bubble::shader::setVec3(const std::string& name, const float& r, const float& g, const float& b) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), r, g, b);
}
void bubble::shader::setVec3(const std::string& name, const vet3& vet) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), vet.x, vet.y, vet.z);
}

void bubble::shader::setVec2(const std::string& name, const float& r, const float& g) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), r, g);
}

bool bubble::shader::checkCompileErrors(unsigned int shader, const std::string& type) {
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

bool bubble::shader::checkLinkErrors(unsigned int program) {
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