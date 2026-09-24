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
 * @file mesh.cpp
 */

#include "util/material.hpp"
using namespace COMMONS_NS;

material::material(std::map<std::string, uniform> unifs) {
    for(auto& [nome, u] : unifs) {
        uniforms[nome] = u;
    }
}
void material::set_uniform(const std::string& nome, const uniform& prop) {
    uniforms[nome] = prop;
}

void material::set_texture(const std::string& nome, const texture& prop) {
    textures[nome] = prop;
}

uniform material::getUniforme(const std::string& nome) const {
    auto it = uniforms.find(nome);
    if(it != uniforms.end())
        return it->second;
    else
        return uniform(1.f);
}

texture material::getTextura(const std::string& nome) const {
    auto it = textures.find(nome);
    if(it != textures.end())
        return it->second;
    else
        return texture();
}
void material::usar(shader& shader) {
    shader.use();

    for (const auto& [nome, prop] : uniforms) {
        switch (prop.m_type) {
            case uniform::type::Float:
                shader.setFloat(nome, prop.valor.f); break;
            case uniform::type::Float_ptr:
                shader.setFloat(nome, *prop.ponteiro.f_ptr); break;

            case uniform::type::Int:
                shader.setInt(nome, prop.valor.i); break;
            case uniform::type::Int_ptr:
                shader.setInt(nome, *prop.ponteiro.i_ptr); break;

            case uniform::type::Bool:
                shader.setBool(nome, prop.valor.b); break;
            case uniform::type::Bool_ptr:
                shader.setBool(nome, *prop.ponteiro.b_ptr); break;

            case uniform::type::Vet2:
                shader.setVec2(nome, prop.valor.vec2.x, prop.valor.vec2.y); break;
            case uniform::type::Vet2_ptr:
                shader.setVec2(nome, prop.ponteiro.vec2_ptr->x, prop.ponteiro.vec2_ptr->y); break;

            case uniform::type::Vet3:
                shader.setVec3(nome, prop.valor.vec3); break;
            case uniform::type::Vet3_ptr:
                shader.setVec3(nome, *prop.ponteiro.vec3_ptr); break;

            case uniform::type::Vet4:
                shader.setVec4(nome, prop.valor.vec4); break;
            case uniform::type::Vet4_ptr:
                shader.setVec4(nome, *prop.ponteiro.vec4_ptr); break;

            case uniform::type::Cor:
                shader.set_color(nome, prop.valor.color_value); break;
            case uniform::type::Cor_ptr:
                shader.set_color(nome, *prop.ponteiro.color_ptr); break;

            case uniform::type::Mat4:
                shader.setMat4(nome, glm::value_ptr(prop.valor.mat4)); break;
            case uniform::type::Mat4_ptr:
                shader.setMat4(nome, glm::value_ptr(*prop.ponteiro.mat4_ptr)); break;
        }
    }

    int slot = 0;
    for (const auto& [nome, tex] : textures) {
        tex.bind(slot);
        shader.setInt(nome, slot);
        ++slot;
    }
}
