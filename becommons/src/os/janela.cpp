
/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file janela.cpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "os/janela.hpp"
#include "depuracao/debug.hpp"
#include "arquivadores/imageloader.hpp"
#include "arquivadores/shader.hpp"
#include "arquivadores/fonte.hpp"

using BECOMMONS_NS;

// Callback de erro
//
//
void errorCallback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

BECOMMONS_NSjanela::~janela()
{
    BECOMMONS_NSdescarregarShaders();
    BECOMMONS_NSgerenciadorFontes::limparFontes();
}
BECOMMONS_NSjanela::janela(const char* nome, bubble::vetor2<double> bounds, const char* icon_path)
{
    glfwSetErrorCallback(errorCallback);
    // inicia glfw
    if (!glfwInit())
    {
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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        depuracao::emitir(erro, "Glad");
        abort();
    }
    
    if(icon_path)
    {
    BECOMMONS_NSimageLoader _icone(icon_path);
    auto glfw_icone = _icone.converterParaGlfw();
    glfwSetWindowIcon(window, 1, &glfw_icone);
    }
    // ativa blend
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONEm_INUS_SRC_ALPHA);
    

    glfwSetCursorPosCallback(window,BECOMMONS_NSmousePosCallBack);
    glfwSetMouseButtonCallback(window, BECOMMONS_NSmouseButtonCallBack);
    glfwSetKeyCallback(window,BECOMMONS_NScallbackKey);
    glfwSetWindowUserPointer(window, this);

    BECOMMONS_NSvetor4<int> tam{};
    glfwGetWindowSize(window, &tam.z, &tam.w);
    tamanho.y = tam.w;
    tamanho.x = tam.z;
}

BECOMMONS_NSjanela::janela(const char* nome, const bool f, bubble::vetor2<double> bounds , const char* icon_path)
{
    glfwSetErrorCallback(errorCallback);
    // inicia glfw
    if (!glfwInit())
    {
        depuracao::emitir(erro, "Iniciando janela glfw");
        abort();
    }
if(f)
    glfwWindowHint(GLFWm_AXIMIZED, GLFW_TRUE);
    window = glfwCreateWindow(bounds.x, bounds.y, nome, NULL, NULL);
    m_nome = nome;
    if (!window) {
        depuracao::emitir(erro, "Janla invalida");
        abort();
    };

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        depuracao::emitir(erro, "Glad");
        abort();
    }
    
    if(icon_path)
    {
    BECOMMONS_NSimageLoader _icone(icon_path);
    auto glfw_icone = _icone.converterParaGlfw();
    glfwSetWindowIcon(window, 1, &glfw_icone);
    }
    // ativa blend
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONEm_INUS_SRC_ALPHA);
    

    glfwSetCursorPosCallback(window,BECOMMONS_NSmousePosCallBack);
    glfwSetMouseButtonCallback(window, BECOMMONS_NSmouseButtonCallBack);
    glfwSetKeyCallback(window,BECOMMONS_NScallbackKey);
    glfwSetWindowUserPointer(window, this);

    BECOMMONS_NSvetor4<int> tam{};
    glfwGetWindowSize(window, &tam.z, &tam.w);
    tamanho.y = tam.w;
    tamanho.x = tam.z;
}
void BECOMMONS_NSjanela::poll()
{
    glfwPollEvents();
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    tamanho.y = h;
    tamanho.x = w;
}

void BECOMMONS_NSjanela::swap() 
{
    if(m_cursor != m_cursor_antigo) 
    {
        auto cursor_glfw = glfwCreateStandardCursor((int)m_cursor);
        if(cursor_glfw)
        glfwSetCursor(window, cursor_glfw);
        m_cursor_antigo = m_cursor;
    }
    m_tempo.calcularDT();
    glfwSwapBuffers(window);
}

void BECOMMONS_NSjanela::viewport() const
{
    glViewport(0, 0, tamanho.x, tamanho.y);
}

void BECOMMONS_NSjanela::nome(const char* novo_nome)
{
    glfwSetWindowTitle(window, novo_nome);
    m_nome = novo_nome;
}

std::string BECOMMONS_NSjanela::nome() const
{
    return m_nome;
}
