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
#include "commons_namespace.hpp"
#include "loaders/shader.hpp"
#include "texture.hpp"
#include "vector2.hpp"
#include "vector3.hpp"
#include "vector4.hpp"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "color.hpp"

namespace COMMONS_NS {
struct uniform {
    enum class type : uint16_t{
        Float, Int, Bool, Vet2, Vet3, Vet4, Cor, Mat4,
        Float_ptr, Int_ptr, Bool_ptr, Vet2_ptr, Vet3_ptr, Vet4_ptr, Cor_ptr, Mat4_ptr
    };
    type m_type;

    union valor_t {
        float f;
        int i;
        bool b;
        fvector_type2 vec2;
        fvector_type3 vec3;
        fvector_type4 vec4;
        color color_value;
        glm::mat4 mat4;

        valor_t() {}  // construtor trivial
        ~valor_t() {} // destrutor manual, necessário com types não triviais
    } valor;

    union ponteiro_t {
        float* f_ptr;
        int* i_ptr;
        bool* b_ptr;
        fvector_type2* vec2_ptr;
        fvector_type3* vec3_ptr;
        fvector_type4* vec4_ptr;
        color* color_ptr;
        glm::mat4* mat4_ptr;

        ponteiro_t() {}
        ~ponteiro_t() {}
    } ponteiro;

    // Constructores
    uniform() : m_type(type::Float) { valor.f = 1.f; }
    uniform(float val) : m_type(type::Float) { valor.f = val; }
    uniform(int val) : m_type(type::Int) { valor.i = val; }
    uniform(bool val) : m_type(type::Bool) { valor.b = val; }
    uniform(const fvector_type2& val) : m_type(type::Vet2) { valor.vec2 = val; }
    uniform(const fvector_type3& val) : m_type(type::Vet3) { valor.vec3 = val; }
    uniform(const fvector_type4& val) : m_type(type::Vet4) { valor.vec4 = val; }
    uniform(const color& val) : m_type(type::Cor) { valor.color_value = val; }
    uniform(const glm::mat4& val) : m_type(type::Mat4) { valor.mat4 = val; }

    uniform(float* val) : m_type(type::Float_ptr) { ponteiro.f_ptr = val; }
    uniform(int* val) : m_type(type::Int_ptr) { ponteiro.i_ptr = val; }
    uniform(bool* val) : m_type(type::Bool_ptr) { ponteiro.b_ptr = val; }
    uniform(fvector_type2* val) : m_type(type::Vet2_ptr) { ponteiro.vec2_ptr = val; }
    uniform(fvector_type3* val) : m_type(type::Vet3_ptr) { ponteiro.vec3_ptr = val; }
    uniform(fvector_type4* val) : m_type(type::Vet4_ptr) { ponteiro.vec4_ptr = val; }
    uniform(color* val) : m_type(type::Cor_ptr) { ponteiro.color_ptr = val; }
    uniform(glm::mat4* val) : m_type(type::Mat4_ptr) { ponteiro.mat4_ptr = val; }

    // Constructor de cópia
    uniform(const uniform& other) : m_type(other.m_type) {
        switch (m_type) {
            case type::Float: valor.f = other.valor.f; break;
            case type::Int: valor.i = other.valor.i; break;
            case type::Bool: valor.b = other.valor.b; break;
            case type::Vet2: valor.vec2 = other.valor.vec2; break;
            case type::Vet3: valor.vec3 = other.valor.vec3; break;
            case type::Vet4: valor.vec4 = other.valor.vec4; break;
            case type::Cor: valor.color_value = other.valor.color_value; break;
            case type::Mat4: valor.mat4 = other.valor.mat4; break;
            case type::Float_ptr: ponteiro.f_ptr = other.ponteiro.f_ptr; break;
            case type::Int_ptr: ponteiro.i_ptr = other.ponteiro.i_ptr; break;
            case type::Bool_ptr: ponteiro.b_ptr = other.ponteiro.b_ptr; break;
            case type::Vet2_ptr: ponteiro.vec2_ptr = other.ponteiro.vec2_ptr; break;
            case type::Vet3_ptr: ponteiro.vec3_ptr = other.ponteiro.vec3_ptr; break;
            case type::Vet4_ptr: ponteiro.vec4_ptr = other.ponteiro.vec4_ptr; break;
            case type::Cor_ptr: ponteiro.color_ptr = other.ponteiro.color_ptr; break;
            case type::Mat4_ptr: ponteiro.mat4_ptr = other.ponteiro.mat4_ptr; break;
        }
    }

    // Operador de cópia
    uniform& operator=(const uniform& other) {
        if (this != &other) {
            m_type = other.m_type;
            switch (m_type) {
                case type::Float: valor.f = other.valor.f; break;
                case type::Int: valor.i = other.valor.i; break;
                case type::Bool: valor.b = other.valor.b; break;
                case type::Vet2: valor.vec2 = other.valor.vec2; break;
                case type::Vet3: valor.vec3 = other.valor.vec3; break;
                case type::Vet4: valor.vec4 = other.valor.vec4; break;
                case type::Cor: valor.color_value = other.valor.color_value; break;
                case type::Mat4: valor.mat4 = other.valor.mat4; break;
                case type::Float_ptr: ponteiro.f_ptr = other.ponteiro.f_ptr; break;
                case type::Int_ptr: ponteiro.i_ptr = other.ponteiro.i_ptr; break;
                case type::Bool_ptr: ponteiro.b_ptr = other.ponteiro.b_ptr; break;
                case type::Vet2_ptr: ponteiro.vec2_ptr = other.ponteiro.vec2_ptr; break;
                case type::Vet3_ptr: ponteiro.vec3_ptr = other.ponteiro.vec3_ptr; break;
                case type::Vet4_ptr: ponteiro.vec4_ptr = other.ponteiro.vec4_ptr; break;
                case type::Cor_ptr: ponteiro.color_ptr = other.ponteiro.color_ptr; break;
                case type::Mat4_ptr: ponteiro.mat4_ptr = other.ponteiro.mat4_ptr; break;
            }
            }
            return *this;
        }
    };

    struct material {
        material() = default;
        material(std::map<std::string, uniform>);
        void set_uniform(const std::string&, const uniform&);
        void set_texture(const std::string&, const texture&);
        uniform getUniforme(const std::string&) const;
        texture getTextura(const std::string&) const;

        std::unordered_map<std::string, texture> textures;
        std::unordered_map<std::string, uniform> uniforms;
        void usar(shader& shader);
    };
}
