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
 * \brief O editor deve disponibilizar uma interface para a manipulação da engine pelo usuário comum. Isso também inclui gerar uma interface gráfica.
 */

#pragma once
#include "becommons/becommons.hpp"
#include "elementos/painel.hpp"

namespace beeditor {
    class sistema_editor : public becommons::sistema {
    public:
        // \brief construtor
        sistema_editor();

        // \brief Funções herdadas
        void inicializar() override;
        void atualizar() override;
        void salvarEditor();
		void abrirProjeto(becommons::projeto*);
        void carregarConfiguracaoPadrao();
        void carregarConfiguracaoGerenciador();
         
        std::shared_ptr<becommons::interface> ui;
        std::shared_ptr<becommons::camera_editor> cam;
        uint32_t m_entidade_selecionada;
        bool m_salvar_ao_fechar;
        
        // Declaração global para acesso rápido às caixas de entidades e inspetor.
        // Essas caixas são gerenciadas pela UI e contêm os elementos visuais
        // para manipulação de entidades e seus componentes.
        becommons::paineis::entity* m_entidades; 
        becommons::paineis::inspector* m_inspetor;
        becommons::paineis::editor* m_editor;
        becommons::paineis::file_manager* m_files;
        becommons::container* dock;
    };
}
