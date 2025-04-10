/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file shader.hpp
 * @brief Gerencia os shaders GLSL.
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 *
 * @see shader.cpp
 */

#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <utility>
#include <unordered_map>
#include <glm/gtc/type_ptr.hpp>
#include "namespace.hpp"
#include "util/vetor4.hpp"
#include "util/vetor2.hpp"
#include "util/vetor3.hpp"
#include "util/cor.hpp"

namespace BECOMMONS_NS {
    class shaderException : public std::exception
    {
    public:
        shaderException(const char* msg);
        virtual const char* what() const noexcept override;
    private:
        std::string msg_;
    };
    
    void descarregarShaders();

    class shader
    {
    public:
        std::string vert, frag;
        // the program ID
        unsigned int ID;

        // constructor reads and builds the shader
        shader() { compilar("assets/shaders/phong.vert", "assets/shaders/phong.frag"); };
        shader(const char* vertexPath, const char* fragmentPath);
        // use/activate the shader
        void use();
        // utility uniform functions
        void compilar(const char* vertexPath, const char* fragmentPath);
        void setBool(const std::string& name, const bool& value) const;
        void setInt(const std::string& name, const int& value) const;
        void setFloat(const std::string& name, const float& value) const;
        void setCor(const std::string& name, const cor& m_cor) const;
        void setVec4(const std::string& name, const vet4& vec4) const;
        void setVec3(const std::string& name, const float& r, const float& g, const float& b) const;
        void setVec3(const std::string &name, const fvet3 &vet) const;
        void setVec2(const std::string &name, const float &r, const float &g) const;
        void setMat4(const std::string& name, const float* value) const;
        void setMat3(const std::string& name, const float* value) const;
        bool checkCompileErrors(unsigned int shader, const std::string& type);
        bool checkLinkErrors(unsigned int shader);
    };
    inline std::unordered_map<std::string, unsigned int> shaders;
}
