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
 * @file painel.hpp
 * @author Daniel O. dos Santos
 */

#pragma once
#include "util/caixa.hpp"
#include "elementos/area_de_toque.hpp"
#include "elementos/caixa_de_texto.hpp"

class tab;
namespace becommons {
    /** \class header: 
     *  \brief `header` é uma caixa que funciona como a aba de uma janela. Pode ser em abas ou janela única.
     */
    class header : public caixa {
    public:
        int tab_atual = 0;
        unsigned int m_tab_l { 60 };
        unsigned int m_tab_h { 16 };
        cor m_cor_ativado { 0.8f, 0.66f, 0.86f, 1.f };
        cor m_cor_desativado { 0.4f, 0.33f, 0.43f, 1.f };
        std::vector<tab*> m_tabs;
        header();
        ~header();
        tipo_caixa tipo() const override { return tipo_caixa::header; };
        void atualizar() override;
        void desenhar(unsigned int) override;
    };
    /** \class painel
     *  \brief `painel` é uma derivação da caixa criada para servir como janela ou dock.
     */
    class painel : public caixa {
    public:
        bool callback_remocao {false};
        bool trigger = true;
        bool m_selecionado {false};
        std::string label;
        painel(const std::string& label = "panel");
        void atualizar() override;
    };

    namespace paineis {
        class text_editor : public painel {
        public:
            elementos::caixa_de_texto* ct;
            elementos::texto* txt;
            text_editor(const std::string&);
            void atualizar() override;
        };
        class coding : public painel {
        public:
            coding();
            void atualizar() override;
            void open(const std::string);
        };
        class file_manager : public painel {
        public:
            std::string m_directory;
            file_manager(const std::string& base_dir);
            void pick_file(const std::string& filtro,
               std::function<void(const std::string)>&& funcao);
            void atualizar() override;
            void research(const std::string&);
        };
        class entity : public painel {
        private:
            bool registro_atualizou = true;
            size_t old_contage = 0;
        public:
            entity();
            void atualizar() override;
        };
        class inspector : public painel {
        public:
            inspector();
        };
        class jogo : public painel {
        public:
            elementos::imagem* framebuffer;
            jogo();
            void play();
            void pause();
            void stop();
            void atualizar() override;
        };
        class editor : public painel {
        public:
            camera_editor* cam;
            elementos::imagem* framebuffer;
            editor(camera_editor* cam);
            void atualizar() override;
        };
    } // paineis
}
