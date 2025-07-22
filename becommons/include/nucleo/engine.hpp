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

/**
 * Inclusão da engine
 */

namespace BECOMMONS_NS {
    enum class exec {
        editor, jogo
    };

    class motor {
    public:
        motor();
        ~motor();
        
        void iniciar();
        void definirDiretorio(const std::string&);
        void rodar(const exec&);
        void finalizar();

        static motor& obter();
        void mainloop();

        bool m_rodando = false;
        exec m_game_mode;
        std::unique_ptr<janela> m_janela;
        std::unique_ptr<sistema_renderizacao> m_renderer;
        std::unique_ptr<inputs> m_inputs;
        std::unique_ptr<tempo> m_tempo;
        //std::unique_ptr<gerenciador_cenas> m_gerenciador_cenas;
        std::string m_games_dir;
    };
}
