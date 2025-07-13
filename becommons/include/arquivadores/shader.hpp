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
 * @file shader.hpp
 * @brief Gerencia os shaders GLSL.
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
#include "becommons_namespace.hpp"
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
        shader() { 
            vert = "phong.vert";
            frag = "phong.frag";
            compilar(); 
        };
        shader(const char* vertexPath, const char* fragmentPath);
        // use/activate the shader
        void use();
        // utility uniform functions
        void compilar();
        void setBool (const std::string& name, const bool& value) const;
        void setInt  (const std::string& name, const int& value) const;
        void setFloat(const std::string& name, const float& value) const;
        void setCor  (const std::string& name, const cor& m_cor) const;
        void setVec4 (const std::string& name, const fvet4& vec4) const;
        void setVec3 (const std::string& name, const float& r, const float& g, const float& b) const;
        void setVec3 (const std::string &name, const fvet3 &vet) const;
        void setVec2 (const std::string &name, const float &r, const float &g) const;
        void setMat4 (const std::string& name, const float* value) const;
        void setMat3 (const std::string& name, const float* value) const;
        bool checkCompileErrors(unsigned int shader, const std::string& type);
        bool checkLinkErrors(unsigned int shader);
    };
    inline std::unordered_map<std::string, unsigned int> shaders;
}
