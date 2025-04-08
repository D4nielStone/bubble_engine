#include <glad/glad.h>

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

#include "GLFW/glfw3.h"
#include "os/janela.hpp"
#include "depuracao/debug.hpp"
#include "arquivadores/imageloader.hpp"
#include "arquivadores/shader.hpp"
#include "arquivadores/fonte.hpp"

using namespace bubble;

// Callback de erro
//
//
void errorCallback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

bubble::janela::~janela()
{
    bubble::descarregarShaders();
    bubble::gerenciadorFontes::limparFontes();
}
bubble::janela::janela(const char* nome, bubble::vetor2<double> bounds, const char* icon_path)
{
    glfwSetErrorCallback(errorCallback);
    // inicia glfw
    if (!glfwInit())
    {
        depuracao::emitir(erro, "Iniciando janela glfw");
        abort();
    }
    
    window = glfwCreateWindow(bounds.x, bounds.y, nome, NULL, NULL);
    _Mnome = nome;
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
    bubble::imageLoader _icone(icon_path);
    auto glfw_icone = _icone.converterParaGlfw();
    glfwSetWindowIcon(window, 1, &glfw_icone);
    }
    // ativa blend
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    

    glfwSetCursorPosCallback(window,bubble::mousePosCallBack);
    glfwSetMouseButtonCallback(window, bubble::mouseButtonCallBack);
    glfwSetKeyCallback(window,bubble::callbackKey);
    glfwSetWindowUserPointer(window, this);

    bubble::vetor4<int> tam{};
    glfwGetWindowSize(window, &tam.z, &tam.w);
    tamanho.y = tam.w;
    tamanho.x = tam.z;
}

bubble::janela::janela(const char* nome, const bool f, bubble::vetor2<double> bounds , const char* icon_path)
{
    glfwSetErrorCallback(errorCallback);
    // inicia glfw
    if (!glfwInit())
    {
        depuracao::emitir(erro, "Iniciando janela glfw");
        abort();
    }
if(f)
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    window = glfwCreateWindow(bounds.x, bounds.y, nome, NULL, NULL);
    _Mnome = nome;
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
    bubble::imageLoader _icone(icon_path);
    auto glfw_icone = _icone.converterParaGlfw();
    glfwSetWindowIcon(window, 1, &glfw_icone);
    }
    // ativa blend
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    

    glfwSetCursorPosCallback(window,bubble::mousePosCallBack);
    glfwSetMouseButtonCallback(window, bubble::mouseButtonCallBack);
    glfwSetKeyCallback(window,bubble::callbackKey);
    glfwSetWindowUserPointer(window, this);

    bubble::vetor4<int> tam{};
    glfwGetWindowSize(window, &tam.z, &tam.w);
    tamanho.y = tam.w;
    tamanho.x = tam.z;
}
void bubble::janela::poll()
{
    glfwPollEvents();
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    tamanho.y = h;
    tamanho.x = w;
}

void bubble::janela::swap() 
{
    if(m_cursor != m_cursor_antigo) 
    {
        auto cursor_glfw = glfwCreateStandardCursor((int)m_cursor);
        if(cursor_glfw)
        glfwSetCursor(window, cursor_glfw);
        m_cursor_antigo = m_cursor;
    }
    _Mtempo.calcularDT();
    glfwSwapBuffers(window);
}

void bubble::janela::viewport() const
{
    glViewport(0, 0, tamanho.x, tamanho.y);
}

void bubble::janela::nome(const char* novo_nome)
{
    glfwSetWindowTitle(window, novo_nome);
    _Mnome = novo_nome;
}

std::string bubble::janela::nome() const
{
    return _Mnome;
}
