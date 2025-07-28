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
#include <map>
#include "arquivadores/shader.hpp"
#include "textura.hpp"
#include "vetor2.hpp"
#include "vetor3.hpp"
#include "vetor4.hpp"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "cor.hpp"

namespace becommons {
struct uniforme {
    enum class tipo : uint16_t{
        Float, Int, Bool, Vet2, Vet3, Vet4, Cor, Mat4,
        Float_ptr, Int_ptr, Bool_ptr, Vet2_ptr, Vet3_ptr, Vet4_ptr, Cor_ptr, Mat4_ptr
    };
    tipo m_tipo;

    union valor_t {
        float f;
        int i;
        bool b;
        fvet2 vec2;
        fvet3 vec3;
        fvet4 vec4;
        cor color;
        glm::mat4 mat4;

        valor_t() {}  // construtor trivial
        ~valor_t() {} // destrutor manual, necessário com tipos não triviais
    } valor;

    union ponteiro_t {
        float* f_ptr;
        int* i_ptr;
        bool* b_ptr;
        fvet2* vec2_ptr;
        fvet3* vec3_ptr;
        fvet4* vec4_ptr;
        cor* color_ptr;
        glm::mat4* mat4_ptr;

        ponteiro_t() {}
        ~ponteiro_t() {}
    } ponteiro;

    // Construtores
    uniforme() : m_tipo(tipo::Float) { valor.f = 1.f; }
    uniforme(float val) : m_tipo(tipo::Float) { valor.f = val; }
    uniforme(int val) : m_tipo(tipo::Int) { valor.i = val; }
    uniforme(bool val) : m_tipo(tipo::Bool) { valor.b = val; }
    uniforme(const fvet2& val) : m_tipo(tipo::Vet2) { valor.vec2 = val; }
    uniforme(const fvet3& val) : m_tipo(tipo::Vet3) { valor.vec3 = val; }
    uniforme(const fvet4& val) : m_tipo(tipo::Vet4) { valor.vec4 = val; }
    uniforme(const cor& val) : m_tipo(tipo::Cor) { valor.color = val; }
    uniforme(const glm::mat4& val) : m_tipo(tipo::Mat4) { valor.mat4 = val; }

    uniforme(float* val) : m_tipo(tipo::Float_ptr) { ponteiro.f_ptr = val; }
    uniforme(int* val) : m_tipo(tipo::Int_ptr) { ponteiro.i_ptr = val; }
    uniforme(bool* val) : m_tipo(tipo::Bool_ptr) { ponteiro.b_ptr = val; }
    uniforme(fvet2* val) : m_tipo(tipo::Vet2_ptr) { ponteiro.vec2_ptr = val; }
    uniforme(fvet3* val) : m_tipo(tipo::Vet3_ptr) { ponteiro.vec3_ptr = val; }
    uniforme(fvet4* val) : m_tipo(tipo::Vet4_ptr) { ponteiro.vec4_ptr = val; }
    uniforme(cor* val) : m_tipo(tipo::Cor_ptr) { ponteiro.color_ptr = val; }
    uniforme(glm::mat4* val) : m_tipo(tipo::Mat4_ptr) { ponteiro.mat4_ptr = val; }

    // Construtor de cópia
    uniforme(const uniforme& other) : m_tipo(other.m_tipo) {
        switch (m_tipo) {
            case tipo::Float: valor.f = other.valor.f; break;
            case tipo::Int: valor.i = other.valor.i; break;
            case tipo::Bool: valor.b = other.valor.b; break;
            case tipo::Vet2: valor.vec2 = other.valor.vec2; break;
            case tipo::Vet3: valor.vec3 = other.valor.vec3; break;
            case tipo::Vet4: valor.vec4 = other.valor.vec4; break;
            case tipo::Cor: valor.color = other.valor.color; break;
            case tipo::Mat4: valor.mat4 = other.valor.mat4; break;
            case tipo::Float_ptr: ponteiro.f_ptr = other.ponteiro.f_ptr; break;
            case tipo::Int_ptr: ponteiro.i_ptr = other.ponteiro.i_ptr; break;
            case tipo::Bool_ptr: ponteiro.b_ptr = other.ponteiro.b_ptr; break;
            case tipo::Vet2_ptr: ponteiro.vec2_ptr = other.ponteiro.vec2_ptr; break;
            case tipo::Vet3_ptr: ponteiro.vec3_ptr = other.ponteiro.vec3_ptr; break;
            case tipo::Vet4_ptr: ponteiro.vec4_ptr = other.ponteiro.vec4_ptr; break;
            case tipo::Cor_ptr: ponteiro.color_ptr = other.ponteiro.color_ptr; break;
            case tipo::Mat4_ptr: ponteiro.mat4_ptr = other.ponteiro.mat4_ptr; break;
        }
    }

    // Operador de cópia
    uniforme& operator=(const uniforme& other) {
        if (this != &other) {
            m_tipo = other.m_tipo;
            switch (m_tipo) {
                case tipo::Float: valor.f = other.valor.f; break;
                case tipo::Int: valor.i = other.valor.i; break;
                case tipo::Bool: valor.b = other.valor.b; break;
                case tipo::Vet2: valor.vec2 = other.valor.vec2; break;
                case tipo::Vet3: valor.vec3 = other.valor.vec3; break;
                case tipo::Vet4: valor.vec4 = other.valor.vec4; break;
                case tipo::Cor: valor.color = other.valor.color; break;
                case tipo::Mat4: valor.mat4 = other.valor.mat4; break;
                case tipo::Float_ptr: ponteiro.f_ptr = other.ponteiro.f_ptr; break;
                case tipo::Int_ptr: ponteiro.i_ptr = other.ponteiro.i_ptr; break;
                case tipo::Bool_ptr: ponteiro.b_ptr = other.ponteiro.b_ptr; break;
                case tipo::Vet2_ptr: ponteiro.vec2_ptr = other.ponteiro.vec2_ptr; break;
                case tipo::Vet3_ptr: ponteiro.vec3_ptr = other.ponteiro.vec3_ptr; break;
                case tipo::Vet4_ptr: ponteiro.vec4_ptr = other.ponteiro.vec4_ptr; break;
                case tipo::Cor_ptr: ponteiro.color_ptr = other.ponteiro.color_ptr; break;
                case tipo::Mat4_ptr: ponteiro.mat4_ptr = other.ponteiro.mat4_ptr; break;
            }
            }
            return *this;
        }
    };

    struct material {
        material() = default;
        material(std::map<std::string, uniforme>);
        void definirUniforme(const std::string&, const uniforme&); 
        void definirTextura(const std::string&, const textura&); 
        uniforme obterUniforme(const std::string&) const;
        textura obterTextura(const std::string&) const;

        std::unordered_map<std::string, textura> texturas;
        std::unordered_map<std::string, uniforme> uniformes;
        void usar(shader& shader);
    };
}
