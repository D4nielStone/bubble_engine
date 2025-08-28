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

namespace becommons {
    class painel : public caixa {
    public:
        bool callback_remocao {false};
        std::string label;
        painel(const std::string& label = "panel");
    };
    /** \class header: 
     *  \brief ´header´ é uma caixa que funciona como a aba de uma janela. Pode ser em abas ou janela única.
     */
    class header : public caixa {
    public:
        header(const std::string& label);
        void atualizar() override;
    };

    class container : public caixa {
    private:
        unsigned int tab_atual{0};
        float porcao = 0.5f;
        bool dividiu{false};
    public:
        container();
        void unsplit();
        std::pair<container*, container*> split(float porcao_inicial = 0.5f, estilo::orientacao o = estilo::orientacao::horizontal);
        void atualizar() override;
        tipo_caixa tipo() const override {return tipo_caixa::container;};
    };

    namespace paineis {
        class file_manager : public painel {
        public:
            file_manager() : painel("file manager") {}
        };
        class entity : public painel {
        private:
            bool registro_atualizou = true;
            uint32_t entidade_selecionada;
            size_t old_contage = 0;
        public:
            entity() : painel("entity register") {}
            uint32_t obter() const { return entidade_selecionada; }
            void atualizar() override;
        };
        class inspector : public painel {
        public:
            inspector();
        };
        class editor : public painel {
        public:
            elementos::imagem* framebuffer;
            editor(camera* cam);
        };
    } // paineis
} // becommons
