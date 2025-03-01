#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <utility>
#include <unordered_map>
#include <glm/gtc/type_ptr.hpp>
#include "util/cor.hpp"
#include "util/vetor4.hpp"
#include "util/vetor3.hpp"

namespace bubble
{
    class shaderException : public std::exception
    {
    public:
        shaderException(const char* msg);
        virtual const char* what() const noexcept override;
    private:
        std::string msg_;
    };

    class shader
    {
    public:
        // the program ID
        unsigned int ID;

        // constructor reads and builds the shader
        shader() { compilar("assets/shaders/phong.vert", "assets/shaders/phong.frag"); };
        shader(const char* vertexPath, const char* fragmentPath);
        // use/activate the shader
        void use() const;
        // utility uniform functions
        void compilar(const char* vertexPath, const char* fragmentPath);
        void setBool(const std::string& name, const bool& value) const;
        void setInt(const std::string& name, const int& value) const;
        void setFloat(const std::string& name, const float& value) const;
        void setCor(const std::string& name, const bubble::cor& cor) const;
        void setVec4(const std::string& name, const vetor4<int>& vec4) const;
        void setVec3(const std::string& name, const float& r, const float& g, const float& b) const;
        void setVec3(const std::string &name, const vet3 &vet) const;
        void setVec2(const std::string &name, const float &r, const float &g) const;
        void setMat4(const std::string& name, const float* value) const;
        void setMat3(const std::string& name, const float* value) const;
        bool checkCompileErrors(unsigned int shader, const std::string& type);
        bool checkLinkErrors(unsigned int shader);
    };
}