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
#include "becommons_namespace.hpp"
#include "sistemas/sistema.hpp"
#include "arquivadores/imageloader.hpp"
#include "arquivadores/fonte.hpp"
#include "util/caixa.hpp"
#include "elementos/botao.hpp"
#include "elementos/caixa_de_texto.hpp"
#include "elementos/imagem.hpp"
#include "elementos/texto.hpp"
#include "util/vetor4.hpp"

namespace BECOMMONS_NS { 
	static unsigned int VAO, VBO, EBO;
    struct interface : sistema {
        void desenhar(caixa*);
        static void atualizarLJ(caixa*);
        static void atualizarAJ(caixa*);
        static void atualizarHDTF(caixa*, std::function<void(caixa*)>);
        static bool deveAtualizar(caixa*);
        void configOpenglState() const;
        void deconfigOpenglState() const;
        void processarDimensaoModular(caixa*, fvet2&, fvet2&);
        void organizarLinha(caixa*, bool, const ivet2 , const fvet2&, const fvet2&, fvet2&);
        void processarModular(caixa*);
        void atualizarFilhos(caixa*);
        void chamarFuncoes(caixa*);
        interface();
        ~interface();

        void gerarBuffers();
        void inicializar() override;
        void atualizar() override;
        
        std::unique_ptr<caixa> m_raiz;
        glm::mat4 projecao_viewport;
    };
}
// \see interface.cpp
