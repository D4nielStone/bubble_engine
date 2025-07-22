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
 * @file main.cpp
 */

#include "nucleo/engine.hpp"
#include "util/versao.hpp"

using namespace BECOMMONS_NS;

motor::motor() : m_tempo(std::make_unique<tempo>()), m_inputs(std::make_unique<inputs>()){
}
motor::~motor() {}

void motor::iniciar() {
    /**
     * logs de inicialização
     */
    depuracao::emitir(debug, "motor", "Olá Bubble!");
    depuracao::emitir(debug, "motor", "Iniciando Bubble Engine " + std::string(BUBBLE_VERSAO_COMPLETA_STR));
    /**
     * inicializa splash screen
     */
    std::thread splash([](){
        auto splash = new janela(janela::splash_screen);
    });
    thread.join();
}
void motor::rodar(const exec& gm) {
    m_game_mode = gm;
    mainloop();
}
void motor::finalizar() {}

motor& becommons::motor::obter() {
    static motor instance;
    return instance;
}
void motor::definirDiretorio(const std::string& path) {
    m_games_dir = path;
}
void motor::mainloop() {
    if(janela::temInstancia())
    while(!janela::deveFechar()) {
        janela::obterInstancia().poll();
        janela::obterInstancia().swap();
    }
}
