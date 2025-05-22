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
 * @file material.hpp
 */

#pragma once
#include <unordered_map>
#include <vector>
#include "becommons_namespace.hpp"
#include "arquivadores/shader.hpp"
#include "textura.hpp"
#include "vetor2.hpp"
#include "vetor3.hpp"
#include "vetor4.hpp"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "cor.hpp"

namespace BECOMMONS_NS {
    struct uniforme {
        enum class tipo { Float, Int, Bool, Vet2, Vet3, Vet4, Cor, Mat4 };
        tipo m_tipo;
    
        union {
            float f;
            int i;
            bool b;
            fvet2 vec2;
            fvet3 vec3;
            fvet4 vec4;
            cor   color;
            glm::mat4 mat4;
        };
    
        uniforme(float val) : m_tipo(tipo::Float), f(val) {}
        uniforme(int val) : m_tipo(tipo::Int), i(val) {}
        uniforme(bool val) : m_tipo(tipo::Bool), b(val) {}
        uniforme(const fvet4& val) : m_tipo(tipo::Vet4), vec4(val) {}
        uniforme(const fvet3& val) : m_tipo(tipo::Vet3), vec3(val) {}
        uniforme(const fvet2& val) : m_tipo(tipo::Vet2), vec2(val) {}
        uniforme(const cor& val) : m_tipo(tipo::Cor), color(val) {}
        uniforme(const glm::mat4& val) : m_tipo(tipo::Mat4), mat4(val) {}
    };

    struct material {
        material() = default;

        material(
                 const std::unordered_map<std::string, textura>& _texturas,
                 const cor& albedo = cor(1.f),
                 float metallic = 0.5f,
                 float roughness = 0.1f,
                 float ao = 1.0f) {configurarPBR(_texturas, albedo, metallic, roughness, ao);}

        void configurarPBR(
                 const std::unordered_map<std::string, textura>& texturas,
                 const cor& albedo = cor(1.f),
                 float metallic = 0.5f,
                 float roughness = 0.1f,
                 float ao = 1.0f);
        void definirUniforme(const std::string&, const uniforme&); 
        void definirTextura(const std::string&, const textura&); 
        uniforme obterUniforme(const std::string&) const;
        textura obterTextura(const std::string&) const;

        std::unordered_map<std::string, textura> texturas;
        std::unordered_map<std::string, uniforme> uniformes;
        void usar(shader& shader);
    };
}
