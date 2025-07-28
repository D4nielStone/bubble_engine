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
 * @file engine.cpp
 */

#include "nucleo/engine.hpp"
#include "util/versao.hpp"

using namespace becommons;

static motor* instance{nullptr};

motor::motor(const std::string& directory) : 
    m_projeto(std::make_shared<projeto>(directory)),
    m_tempo(std::make_shared<tempo>()),
    m_inputs(std::make_shared<inputs>()),
    m_renderer(std::make_shared<sistema_renderizacao>()),
    m_fisica(std::make_shared<sistema_fisica>()),
    m_codigo(std::make_shared<sistema_codigo>()) {
    instance = this;
    m_levelmanager = std::make_shared<levelmanager>(m_projeto.get());
}
motor::~motor() {}

void motor::iniciar(const exec& gm) {
    /**
     * logs de inicialização
     */
    depuracao::emitir(debug, "motor", "Olá Mundo!");
    depuracao::emitir(debug, "motor", "Iniciando Bubble Engine " + std::string(BUBBLE_VERSAO_COMPLETA_STR) + " ExecMode:" + (char)gm);

    m_game_mode = gm;
    /**
     * inicializa splash screen
     
    std::thread splash([](){
        auto splash = new janela(janela::splash_screen);
    });
    thread.join();
     */
    if(m_game_mode == exec::jogo) {
        criarJanela();
        m_interface = std::make_shared<interface>(m_janela.get());
        m_levelmanager->carregar(m_projeto->m_lancamento);
        m_levelmanager->obterFaseAtual()->iniciar();
    }
}
void motor::rodar() {
    while(!m_janela->deveFechar()) {
        m_janela->poll();
    	// Atualiza e renderiza fase atual
        if(m_levelmanager->obterFaseAtual()) {
            // Executa funções opengl de outras threads
            while(!fila_opengl.empty()) {
                auto func = fila_opengl.front();
                func();
                fila_opengl.pop();
            }
    
            // Atualiza/Inicializa sistemas pra quando a fase for iniciada
            if (m_levelmanager->obterFaseAtual()->rodando) {
                if(!m_codigo->init) m_codigo->inicializar();
                m_codigo->atualizar();        
                if(!m_fisica->init) m_fisica->inicializar();
                m_fisica->atualizar();        
            }
    
            // Inicializa sistema de renderização
            if (!m_renderer->init) m_renderer->inicializar();
            m_renderer->atualizar();        
                
            /*if(obterFaseAtual()->rodando) {
                if(!m_interface.init) {
                    m_interface.inicializar();
                }
                    m_interface->atualizar();    
            }*/
        }
        // Atualiza sistemas complementares
        for (auto& s : sistemas) {
            if(!s->init) s->inicializar();
            s->atualizar();        
        }
        m_janela->swap();
    }
}

void motor::finalizar() {
}

motor& becommons::motor::obter() {
    return *instance;
}
        
exec motor::obterTipo() const {
    return m_game_mode;
}
        
bool motor::rodando() const {
    return m_rodando;
}

void motor::criarJanela() {
    m_janela = std::make_shared<janela>(
        m_projeto->m_nome_janela.c_str(),
        false,
        fvet2(m_projeto->m_largura,
              m_projeto->m_altura),
             (m_projeto->m_diretorio + "/" + m_projeto->m_icone).c_str());
}
