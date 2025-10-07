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
        float m_porcao = 0.5f;
        bool m_dividiu{false},
             m_sou_a{false},
             m_floating{false};
        container* m_container_pai{nullptr};
        elementos::popup* m_popup_close{nullptr};
        container *m_a{nullptr}, *m_b{nullptr};
    public:
        fvet4 m_limite_flutuando{100,100,300,50};
        container(bool floating = false);
        header* gerarHeader();
        // tabulaçãopara novos paineis
        template <typename T, typename ...Args>
        T* nova_tab(Args&&... args) {
            if(m_dividiu) return nullptr;
            caixa* h;
            m_filhos.empty() ? h = adicionar<header>() : h = m_filhos[0].get();
            auto* ptr = h->adicionar<T>(std::forward<Args>(args)...);
            return ptr;
        }
        void unsplit(bool);
        std::pair<container*, container*> split(float porcao_inicial = 0.5f, estilo::orientacao o = estilo::orientacao::horizontal);
        void atualizar() override;
        tipo_caixa tipo() const override {return tipo_caixa::container;};
    };
} // becommons
