/** \copyright 
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
 * \file drawcall.hpp
 */

#pragma once
#include "material.hpp"
namespace becommons {
	struct draw_call {
	    GLuint m_vao = 0;
	    GLenum m_mode = GL_TRIANGLES;
	    GLsizei m_count = 0;
	    GLenum m_type = GL_UNSIGNED_INT;
	    const void* m_indices = nullptr;
	
	    GLuint m_program; // referencia do shader
	    material* m_material; // referencia do material/uniforms
    };
    // \class `pipeline`
    // \brief Gerencia os draw calls facilitando a renderização.
    class pipeline {
    public:
        pipeline();
        void addDrawCall(const unsigned int layer, draw_call* ref);
        void limparLayer(const unsigned int layer);
        void limpar();
        void chamarDrawCalls();
    ptivare:
        std::map<unsigned int, std::vector<draw_call*>> m_draw_calls;
    };
}
