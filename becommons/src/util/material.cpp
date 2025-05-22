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

void material::configurarPBR(
                 const std::unordered_map<std::string, textura>& texturas,
                 const cor& albedo = cor(1.f),
                 float metallic = 0.5f,
                 float roughness = 0.1f,
                 float ao = 1.0f) {
    for(auto [nome, tex] : texturas) {
        definirTextura(nome, tex);
        definirUniforme("use_" + nome, true);
    }
    definirUniforme("recebe_luz", true);
    definirUniforme("material.albedo", albedo);
    definirUniforme("material.metallic", metallic);
    definirUniforme("material.ao", ao);
    definirUniforme("material.roughness", roughness);
    definirUniforme("uvMundo", false);
}
void material::definirUniforme(const std::string& nome, const uniforme& prop) {
    uniformes[nome] = prop;
}

void material::definirTextura(const std::string& nome, const textura& prop) {
    texturas[nome] = prop;
}

uniforme material::obterUniforme(const std::string& nome) const {
    return uniformes[nome];
}
    
textura material::obterTextura(const std::string& nome) const {
    return texturas[nome];
}
void material::usar(shader& shader) {
    for (const auto& [nome, prop] : uniformes) {
        switch (prop.m_tipo) {
            case uniforme::tipo::Float:
                shader.setFloat(nome, prop.f); break;
            case uniforme::tipo::Int:
                shader.setInt(nome, prop.i); break;
            case uniforme::tipo::Cor:
                shader.setCor(nome, prop.color); break;
            case uniforme::tipo::Vet2:
                shader.setBool(nome, prop.b); break;
            case uniforme::tipo::Vet2:
                shader.setVec2(nome, prop.vec2); break;
            case uniforme::tipo::Vet3:
                shader.setVec3(nome, prop.vec3); break;
            case uniforme::tipo::Vet4:
                shader.setVec4(nome, prop.vec4); break;
            case uniforme::tipo::Mat4:
                shader.setMat4(nome, glm::value_ptr(prop.mat4)); break;
        }
    }
    int slot = 0;
    for (const auto& [nome, tex] : texturas) {
        tex.bind(slot);
        shader.setInt(nome, slot);
        ++slot;
    }
}
