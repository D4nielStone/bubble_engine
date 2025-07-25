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
 * @file janela.cpp
 */

#include <stdexcept>
#include "becommons_namespace.hpp"
#include "glad.h"
#include "GLFW/glfw3.h"
#include "os/janela.hpp"
#include "nucleo/engine.hpp"
#include "depuracao/debug.hpp"
#include "sistemas/interface.hpp"
#include "arquivadores/imageloader.hpp"
#include "arquivadores/shader.hpp"
#include "arquivadores/fonte.hpp"
#include "inputs/inputs.hpp"

using namespace BECOMMONS_NS;

static janela* instanciaAtual {nullptr};

// Callback de erro
void errorCallback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}
void janela::definirCursor(const cursor c) {
    m_cursor = c;
}

bool janela::temInstancia() {
    return instanciaAtual;
}

janela& janela::obterInstancia() {
    if (!temInstancia())
       throw std::runtime_error("Instância da janela não foi gerada!");
    return *instanciaAtual;
}
            
bool janela::deveFechar() {
    return glfwWindowShouldClose(janela::obterInstancia().window);
}

void janela::gerarInstancia(const configuracao cfg) {
    if(instanciaAtual) delete instanciaAtual;
    instanciaAtual = new janela(cfg);
}
void janela::gerarInstancia(const char* nome, fvet2 bounds , const char* icon_path ) {
    if(instanciaAtual) delete instanciaAtual;
    instanciaAtual = new janela(nome, bounds, icon_path);
}
void janela::gerarInstancia(const char* nome, const bool f, fvet2 bounds , const char* icon_path ) {
    if(instanciaAtual) delete instanciaAtual;
    instanciaAtual = new janela(nome, f, bounds, icon_path);
}

ivet2 janela::obterTamanhoJanela() {
    return janela::obterInstancia().tamanho;
};

void janela::posicionarCursor(double x, double y) {
    auto& input = motor::obter().m_inputs;
    
    input->m_mousex = x;
    input->m_mousey = y;
    glfwSetCursorPos(janela::obterInstancia().window, x, y);
}

janela::~janela() {
    descarregarShaders();
    gerenciadorFontes::limparFontes();
}

janela::janela(const configuracao cfg) {
    switch (cfg) {
        case janela::padrao:
            janela::gerarInstancia("default");
        break;  
        case janela::splash_screen:
            const char* icon_path = "icon.ico";
            const char *nome = "splash";
            fvet2 bounds = {50.f, 50.f};
            /* splash screen */        
            glfwSetErrorCallback(errorCallback);
            // inicia glfw
            if (!glfwInit()) {
                depuracao::emitir(erro, "Iniciando janela glfw");
                abort();
            }
            
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
            glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
           
            window = glfwCreateWindow(bounds.x, bounds.y, nome, NULL, NULL);
            m_nome = nome;
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            glfwSetWindowPos(window,
                 (mode->width - bounds.x) / 2,
                 (mode->height - bounds.y) / 2);

            if (!window) {
                depuracao::emitir(erro, "Janla invalida");
                abort();
            };
         
            glfwMakeContextCurrent(window);
         
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                depuracao::emitir(erro, "Glad");
                abort();
            }
            
         
            if(icon_path) {
            imageLoader _icone(icon_path);
            auto glfw_icone = _icone.converterParaGlfw();
            glfwSetWindowIcon(window, 1, &glfw_icone);
            }
            // ativa blend
            glEnable(GL_BLEND); 
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         
            glEnable(GL_FRAMEBUFFER_SRGB);    
         
            glfwSetCursorPosCallback(window,mousePosCallback);
            glfwSetCharCallback(window, charCallback);
            glfwSetMouseButtonCallback(window, mouseButtonCallback);
            glfwSetKeyCallback(window,keyCallback);
            glfwSetWindowUserPointer(window, this);
         
            vetor4<int> tam{};
            glfwGetWindowSize(window, &tam.z, &tam.w);
            tamanho.y = tam.w;
            tamanho.x = tam.z;

            interface ui(this);
            ui.inicializar();
            ui.m_raiz = std::make_unique<elementos::imagem>("icon.ico");

            while(!glfwWindowShouldClose(window)) {
                glfwPollEvents();
                ui.atualizar();
                glfwSwapBuffers(window);
            }
    }
}

janela::janela(const char* nome, fvet2 bounds, const char* icon_path) {
    glfwSetErrorCallback(errorCallback);
    // inicia glfw
    if (!glfwInit()) {
        depuracao::emitir(erro, "Iniciando janela glfw");
        abort();
    }
    
    window = glfwCreateWindow(bounds.x, bounds.y, nome, NULL, NULL);
    m_nome = nome;
    if (!window) {
        depuracao::emitir(erro, "Janla invalida");
        abort();
    };

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        depuracao::emitir(erro, "Glad");
        abort();
    }
    

    if(icon_path) {
    imageLoader _icone(icon_path);
    auto glfw_icone = _icone.converterParaGlfw();
    glfwSetWindowIcon(window, 1, &glfw_icone);
    }
    // ativa blend
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_FRAMEBUFFER_SRGB);    

    glfwSetCursorPosCallback(window,mousePosCallback);
    glfwSetCharCallback(window, charCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window,keyCallback);
    glfwSetWindowUserPointer(window, this);

    vetor4<int> tam{};
    glfwGetWindowSize(window, &tam.z, &tam.w);
    tamanho.y = tam.w;
    tamanho.x = tam.z;
}

janela::janela(const char* nome, const bool f, fvet2 bounds , const char* icon_path) {
    glfwSetErrorCallback(errorCallback);
    // inicia glfw
    if (!glfwInit()) {
        depuracao::emitir(erro, "Iniciando janela glfw");
        abort();
    }
if(f)
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    window = glfwCreateWindow(bounds.x, bounds.y, nome, NULL, NULL);
    m_nome = nome;
    if (!window) {
        depuracao::emitir(erro, "Janla invalida");
        abort();
    };

    glfwMakeContextCurrent(window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        depuracao::emitir(erro, "Glad");
        abort();
    }
    
    // obtém verção
    const GLubyte* versao = glGetString(GL_VERSION);    
    if(!versao) depuracao::emitir(erro, "Versão opengl não identificade.");
    else {
        depuracao::emitir(info, "Versão opengl: " + std::string(reinterpret_cast<const char*>(versao)) + ".");
        int major = versao[0];
        // usar opengl legacy opengl < 3.0.
        // máquinas antigas
        if(major < 3) {
            depuracao::emitir(alerta, "Opengl legacy detectado. Ativando modo de compatibilidade.");
            //modoLegado();
        }
    }

    if(icon_path) {
        imageLoader _icone(icon_path);
        auto glfw_icone = _icone.converterParaGlfw();
        glfwSetWindowIcon(window, 1, &glfw_icone);
    }
    // ativa blend
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glfwSetCursorPosCallback(window,mousePosCallback);
    glfwSetCharCallback(window, charCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window,keyCallback);
    glfwSetWindowUserPointer(window, this);

    glfwGetWindowSize(window, &tamanho.x, &tamanho.y);
}
void janela::poll() {
    motor::obter().m_inputs->m_backspace_pressionado = false;
    motor::obter().m_inputs->m_backspace_repetido = false;

    glfwPollEvents();
    glfwGetWindowSize(window, &tamanho.x, &tamanho.y);
}

void janela::swap()  {
    if(m_cursor != m_cursor_antigo)  {
        auto cursor_glfw = glfwCreateStandardCursor((int)m_cursor);
        if(cursor_glfw)
        glfwSetCursor(window, cursor_glfw);
        m_cursor_antigo = m_cursor;
    }
    motor::obter().m_inputs->m_letra_pressionada = false;
    glfwSwapBuffers(window);
    motor::obter().m_tempo->calcularDT();
}

void janela::viewport() const {
    glViewport(0, 0, tamanho.x, tamanho.y);
}

void janela::nome(const char* novo_nome) {
    glfwSetWindowTitle(window, novo_nome);
    m_nome = novo_nome;
}

std::string janela::nome() const {
    return m_nome;
}
