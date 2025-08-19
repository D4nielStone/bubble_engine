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
 * LIABILITY, WHETHER IN AN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * @file engine.hpp
 *
 * - Processar input do sistema operacional (janela, eventos).
 *
 * - Atualizar a lógica do jogo (física, scripts, animações).
 *
 * - Renderizar a cena.
 *
 * - Repetir.
 */

#pragma once
#include <string>
#include <memory>
#include <filesystem>
#include "depuracao/debug.hpp"
#include "os/janela.hpp"
#include "os/tempo.hpp"
#include "inputs/inputs.hpp"
#include "sistemas/sistema_de_renderizacao.hpp"
#include "sistemas/interface.hpp"
#include "sistemas/sistema_de_codigo.hpp"
#include "sistemas/sistema_de_fisica.hpp"
#include "levelmanager.hpp"
#include "nucleo/projeto.hpp"
#include "sistemas/editor.hpp"

namespace becommons {
    enum class exec : char {
        editor =    'E',
        jogo =      'G'
    };

    class motor {
    private:
        void criarJanela();
        bool m_rodando = false;
        exec m_game_mode;
    public:
        // \brief Construtor padrão
        // \param diretório do projeto à ser trabalhado
        motor(const std::string&);
        ~motor();
        motor();
        
        void iniciar(const exec&);
        void rodar();
        void finalizar();

        static motor& obter();

        exec obterTipo() const;
        bool rodando() const;

        std::shared_ptr<janela> m_janela;
        std::shared_ptr<projeto> m_projeto;
        std::shared_ptr<sistema_renderizacao> m_renderer;
        std::shared_ptr<sistema_fisica> m_fisica;
        std::shared_ptr<beeditor::sistema_editor> m_editor;
        std::shared_ptr<sistema_codigo> m_codigo;
        std::shared_ptr<interface> m_interface;
        std::shared_ptr<inputs> m_inputs;
        std::shared_ptr<tempo> m_tempo;
        std::shared_ptr<levelmanager> m_levelmanager;
        // \brief Fila para chamadas opengl na thread principal
        std::queue<std::function<void()>> fila_opengl;
    };
}
