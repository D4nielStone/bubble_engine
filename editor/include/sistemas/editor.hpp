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
 * @file editor.hpp
 */

#pragma once
#include "editor_namespace.hpp"
#include "becommons/becommons.hpp"
#include <thread>
#include <atomic>

namespace EDITOR_NS {
    struct sistema_editor : becommons::sistema {
        // \brief construtor
        sistema_editor();

        // \brief funcoes
        void inicializar() override;
        void atualizar() override;
        void adicionarCaixas();
        void chamarInputs();
        void salvarEditor();

        // \brief thread
        static void executarRuntime();
        inline static std::thread threadRuntime;
        inline static std::atomic<bool> rodando{false};
        static void monitorarRuntime();
         
        // \brief atributos
        becommons::camera_editor cam;
        becommons::interface ui;
        size_t num_entidades_anterior = 0;
        size_t entidade_anterior = 0;
        size_t entidade_atual = 1;
};
}
