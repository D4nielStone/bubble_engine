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
 * @file area_de_texto.hpp
 * @author Daniel O. dos Santos
 */

#pragma once
#include "util/caixa.hpp"
#include "elementos/imagem.hpp"
#include "elementos/painel.hpp"

namespace becommons {
    class container : public caixa {
    private:
        float porcao = 0.5f;
        bool dividiu{false};
    public:
        container();
        template <typename T, typename ...Args>
        T* tab(Args&&... args) {
            if(dividiu) return nullptr;
            caixa* h;
            m_filhos.empty() ? h = adicionar<header>() : h = m_filhos[0].get();
            auto* ptr = h->adicionar<T>(std::forward<Args>(args)...);
            return ptr;
        }
        void unsplit();
        std::pair<container*, container*> split(float porcao_inicial = 0.5f, estilo::orientacao o = estilo::orientacao::horizontal);
        void atualizar() override;
        tipo_caixa tipo() const override {return tipo_caixa::container;};
    };
} // becommons
