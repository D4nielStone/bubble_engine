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
 * \file sistema_interface.hpp
 */

#pragma once
#include <glad.h>
#include <memory>
#include <set>
#include "sistemas/sistema.hpp"
#include "arquivadores/imageloader.hpp"
#include "arquivadores/fonte.hpp"
#include "util/caixa.hpp"
#include "elementos/botao.hpp"
#include "elementos/caixa_de_texto.hpp"
#include "elementos/imagem.hpp"
#include "elementos/texto.hpp"
#include "util/vetor4.hpp"
#include "os/janela.hpp"

namespace becommons { 
	static GLuint VAO, VBO, EBO;
    struct interface : sistema {
        void desenhar(caixa*);
        void atualizarLJ(caixa*);
        void atualizarAJ(caixa*);
        void atualizarHDTF(caixa*, std::function<void(caixa*)>);
        void chamarFuncoes(caixa*);
        void configOpenglState() const;
        bool deveAtualizar(caixa*);
        void deconfigOpenglState() const;
        void processarDimensaoModular(caixa*, fvet2&, fvet2&);
        void organizarLinha(caixa*, bool, const ivet2 , const fvet2&, const fvet2&, fvet2&);
        void processarModular(caixa*);
        void atualizarFilhos(caixa*);
        interface();
        interface(ijanela*);
        ~interface();

        void gerarBuffers();
        void inicializar() override;
        void atualizar() override;
        void renderizar();
    
        caixa* obterRaiz();
        std::vector<caixa*> pos_render;
        std::unique_ptr<caixa> m_raiz;
        glm::mat4 projecao_viewport;
        ijanela* m_window = nullptr;
    };
}
// \see interface.cpp
