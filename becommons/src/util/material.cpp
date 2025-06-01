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
 */
/**
 * @file malha.cpp
 */

#include "util/material.hpp"
using namespace BECOMMONS_NS;

material::material(std::map<std::string, uniforme> unifs) {
    for(auto& [nome, u] : unifs) {
        uniformes[nome] = u;
    }
}
void material::definirUniforme(const std::string& nome, const uniforme& prop) {
    uniformes[nome] = prop;
}

void material::definirTextura(const std::string& nome, const textura& prop) {
    texturas[nome] = prop;
}

uniforme material::obterUniforme(const std::string& nome) const {
    auto it = uniformes.find(nome);
    if(it != uniformes.end())
        return it->second;
    else
        return uniforme(1.f);
}
    
textura material::obterTextura(const std::string& nome) const {
    auto it = texturas.find(nome);
    if(it != texturas.end())
        return it->second;
    else
        return textura();
}
void material::usar(shader& shader) {
    shader.use();

    for (const auto& [nome, prop] : uniformes) {
        switch (prop.m_tipo) {
            case uniforme::tipo::Float:
                shader.setFloat(nome, prop.valor.f); break;
            case uniforme::tipo::Float_ptr:
                shader.setFloat(nome, *prop.ponteiro.f_ptr); break;

            case uniforme::tipo::Int:
                shader.setInt(nome, prop.valor.i); break;
            case uniforme::tipo::Int_ptr:
                shader.setInt(nome, *prop.ponteiro.i_ptr); break;

            case uniforme::tipo::Bool:
                shader.setBool(nome, prop.valor.b); break;
            case uniforme::tipo::Bool_ptr:
                shader.setBool(nome, *prop.ponteiro.b_ptr); break;

            case uniforme::tipo::Vet2:
                shader.setVec2(nome, prop.valor.vec2.x, prop.valor.vec2.y); break;
            case uniforme::tipo::Vet2_ptr:
                shader.setVec2(nome, prop.ponteiro.vec2_ptr->x, prop.ponteiro.vec2_ptr->y); break;

            case uniforme::tipo::Vet3:
                shader.setVec3(nome, prop.valor.vec3); break;
            case uniforme::tipo::Vet3_ptr:
                shader.setVec3(nome, *prop.ponteiro.vec3_ptr); break;

            case uniforme::tipo::Vet4:
                shader.setVec4(nome, prop.valor.vec4); break;
            case uniforme::tipo::Vet4_ptr:
                shader.setVec4(nome, *prop.ponteiro.vec4_ptr); break;

            case uniforme::tipo::Cor:
                shader.setCor(nome, prop.valor.color); break;
            case uniforme::tipo::Cor_ptr:
                shader.setCor(nome, *prop.ponteiro.color_ptr); break;

            case uniforme::tipo::Mat4:
                shader.setMat4(nome, glm::value_ptr(prop.valor.mat4)); break;
            case uniforme::tipo::Mat4_ptr:
                shader.setMat4(nome, glm::value_ptr(*prop.ponteiro.mat4_ptr)); break;
        }
    }

    int slot = 0;
    for (const auto& [nome, tex] : texturas) {
        tex.bind(slot);
        shader.setInt(nome, slot);
        ++slot;
    }
}
