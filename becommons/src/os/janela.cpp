
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

#include "namespace.hpp"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "os/janela.hpp"
#include "depuracao/debug.hpp"
#include "arquivadores/imageloader.hpp"
#include "arquivadores/shader.hpp"
#include "arquivadores/fonte.hpp"
#include "inputs/inputs.hpp"

using namespace BECOMMONS_NS;

// Callback de erro
void errorCallback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

janela::~janela()
{
    descarregarShaders();
    gerenciadorFontes::limparFontes();
}
janela::janela(const char* nome, BECOMMONS_NS::vetor2<double> bounds, const char* icon_path)
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
    imageLoader _icone(icon_path);
    auto glfw_icone = _icone.converterParaGlfw();
    glfwSetWindowIcon(window, 1, &glfw_icone);
    }
    // ativa blend
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    

    glfwSetCursorPosCallback(window,mousePosCallBack);
    glfwSetMouseButtonCallback(window, mouseButtonCallBack);
    glfwSetKeyCallback(window,callbackKey);
    glfwSetWindowUserPointer(window, this);

    vetor4<int> tam{};
    glfwGetWindowSize(window, &tam.z, &tam.w);
    tamanho.y = tam.w;
    tamanho.x = tam.z;
}

janela::janela(const char* nome, const bool f, BECOMMONS_NS::vetor2<double> bounds , const char* icon_path)
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
    imageLoader _icone(icon_path);
    auto glfw_icone = _icone.converterParaGlfw();
    glfwSetWindowIcon(window, 1, &glfw_icone);
    }
    // ativa blend
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    

    glfwSetCursorPosCallback(window,mousePosCallBack);
    glfwSetMouseButtonCallback(window, mouseButtonCallBack);
    glfwSetKeyCallback(window,callbackKey);
    glfwSetWindowUserPointer(window, this);

    vetor4<int> tam{};
    glfwGetWindowSize(window, &tam.z, &tam.w);
    tamanho.y = tam.w;
    tamanho.x = tam.z;
}
void janela::poll()
{
    glfwPollEvents();
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    tamanho.y = h;
    tamanho.x = w;
}

void janela::swap() 
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

void janela::viewport() const
{
    glViewport(0, 0, tamanho.x, tamanho.y);
}

void janela::nome(const char* novo_nome)
{
    glfwSetWindowTitle(window, novo_nome);
    m_nome = novo_nome;
}

std::string janela::nome() const
{
    return m_nome;
}
