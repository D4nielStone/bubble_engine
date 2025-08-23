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

static std::unique_ptr<elementos::imagem> s_quad_principal;
static motor* instance{nullptr};

motor::motor() : 
    m_tempo(std::make_shared<tempo>()),
    m_inputs(std::make_shared<inputs>()),
    m_renderer(std::make_shared<sistema_renderizacao>()),
    m_fisica(std::make_shared<sistema_fisica>()),
    m_codigo(std::make_shared<sistema_codigo>()) {
    instance = this;
}

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
    // cria janela
    criarJanela();
    // framebuffer da câmera
    s_quad_principal = std::make_unique<elementos::imagem>(0, true);
    s_quad_principal->configurar();
    // sistema de ui do runtime
    m_interface = std::make_shared<interface>(m_janela.get());

    if(m_game_mode == exec::jogo) {
        // carrega fase
        m_levelmanager->carregar(m_projeto->m_lancamento);
        m_levelmanager->obterFaseAtual()->iniciar();
    }
    else 
        // sistema de editor
        m_editor = std::make_shared<beeditor::sistema_editor>();
}

void motor::atualizarSistemas() {
    // Atualiza e renderiza fase atual
    if(m_levelmanager && m_levelmanager->obterFaseAtual() && !m_levelmanager->carregando()) {
        // Atualiza/Inicializa sistemas pra quando a fase for iniciada
        if (m_levelmanager->obterFaseAtual()->rodando) {
            if(!m_codigo->init) m_codigo->inicializar();
            m_codigo->atualizar();        
            if(!m_fisica->init) m_fisica->inicializar();
            m_fisica->atualizar();         
            if (!m_renderer->init) m_renderer->inicializar();
            m_renderer->atualizar();        
        }
    }
        
    if(m_game_mode == exec::editor) {
        if(!m_editor->init) m_editor->inicializar();
        m_editor->atualizar();    
    }
}

void motor::processarFuncoesOpengl() {
    // Executa funções opengl de outras threads
    while(!fila_opengl.empty()) {
        auto func = fila_opengl.front();
        func();
        fila_opengl.pop();
    }
}

void motor::shadowPass() {
    // Baseado na coleta de luzes e malhas, atualiza o buffer das sombras
    // TODO: update do framebuffer de cada iluminação
}
void motor::geometryPass(camera* cam) {
    // Atualiza o framebuffer da camera principal
    // cam->desenharFB();
    // cam->m_pipeline.chamarDrawCalls();
    // cam->m_ui.m_pipeline.chamarDrawCalls();
}
void motor::uiPass(camera* cam) {
    // Renderiza a interface do jogo dentro do fb da câmera
    // m_interface->m_pipeline.chamarDrawCalls();
    // cam->limparFB();
}     

void motor::rodar() {
    while(!m_janela->deveFechar()) {
        m_janela->poll();

        // As chamadas opengl de outras threads, como criação de buffers, devem ser atualizadas aqui.
        processarFuncoesOpengl();
        // Aqui é atualizada a lógica do jogo nos sistemas de codificação e física.
        // NOTE: os sistemas não devem desenhar de fato na tela, apenas efetuar a coleta dos draw calls.
        /* [Renderer::coletar] → pega malhas, luzes, câmeras
                 ↓
        [Renderer::atualizar] → calcula matrizes e prepara dados
                 ↓
        [Loop Principal]
            → Shadow Pass (luzes com sombra)
            → Geometry Pass (câmera principal)
            → UI Pass (interface + composições)
            
            → Swap buffers
        */
        // \{ câmera ativa:
        atualizarSistemas();
    
        if(m_levelmanager && m_levelmanager->obterFaseAtual() && !m_levelmanager->carregando()) {
        
            shadowPass();   // Atualiza os buffers das sombras
            // camera* cam = m_renderer->obterCameraPrincipal()
            geometryPass(m_renderer->obterCameraPrincipal()); // tualiza o buffer com as entidades(visíveis pela câmera ativa) da cena
            uiPass(m_renderer->obterCameraPrincipal());       // Esse uiPass serve para a interface em modo de jogo ( HUD, Barra de vida etc. 
                                                              // Não confundir com a interface principal ).
        // \}
        }
        // Atualiza as drawcalls da pipeline principal
        // Responsável por desenhar a interface gráica. Isso depende do modo de execução. Exemplo:
        // editor: desenha interface, mensagems de alerta, guizmos e pode desenhar extras, como linhas,
        // quads, triangulos, malhas e outras formas.
        // jogo:  desenha um quadrado com o framebuffer do jogo. Também pode incluir um modo debug,
        // que desenha informações como fps, uso de memória etc.
        
        if(m_game_mode == exec::editor) m_editor->ui->m_pipeline.chamarDrawCalls();
        else {
            float w = m_janela->tamanho.x;
            float h = m_janela->tamanho.y;
            s_quad_principal->m_projecao = glm::ortho(0.f, w, h, 0.f);
            s_quad_principal->id = m_renderer->obterCameraPrincipal()->textura;
            s_quad_principal->m_estilo.m_largura = w;
            s_quad_principal->m_estilo.m_altura = h;
            draw_call dc = s_quad_principal->obterDrawCall();
            pipeline::chamarDrawCall(&dc);
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
    if(m_projeto) {
    m_janela = std::make_shared<janela>(
        m_projeto->m_nome_janela.c_str(),
        false,
        fvet2(m_projeto->m_largura,
              m_projeto->m_altura),
             (m_projeto->m_diretorio + "/" + m_projeto->m_icone).c_str());

    } else {
    
    m_janela = std::make_shared<janela>(
	"BE: Editor © BubbleStudio",
        true,
        fvet2(640,
              480),
             "icon.ico");

    }
}
