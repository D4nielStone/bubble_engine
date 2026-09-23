#include <stdexcept>
#include "commons_namespace.hpp"
#include "glad.h"
#include "GLFW/glfw3.h"
#include "os/window.hpp"
#include "depuracao/debug.hpp"
#include "arquivadores/imageloader.hpp"
#include "arquivadores/shader.hpp"
#include "arquivadores/fonte.hpp"
#include "inputs/inputs.hpp"

using namespace COMMONS_NS;

void errorCallback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

bool window::temInstancia() {
    return instanciaAtual;
}

window& window::obterInstancia() {
    if (!temInstancia())
       throw std::runtime_error("Instância da window não foi gerada!");
    return *instanciaAtual;
}
window& window::newInstance(const char* nome, fvet2 bounds , const char* icon_path ) {
    if(instanciaAtual) delete instanciaAtual;
    instanciaAtual = new window(nome, bounds, icon_path);
    return *instanciaAtual;
}
window& window::newInstance(const char* nome, const bool f, fvet2 bounds , const char* icon_path ) {
    if(instanciaAtual) delete instanciaAtual;
    instanciaAtual = new window(nome, f, bounds, icon_path);
    return *instanciaAtual;
}

ivet2 window::obterTamanhoWindow() {
    return window::obterInstancia().tamanho;
};

void window::posicionarCursor(double x, double y)
{
    auto& input = window::obterInstancia().m_inputs;
    
    input.m_mousex = x;
    input.m_mousey = y;
    glfwSetCursorPos(window::obterInstancia().m_window, x, y);
}

window::~window() {
    descarregarShaders();
    gerenciadorFontes::limparFontes();
}
window::window(const char* nome, fvet2 bounds, const char* icon_path)
{
    glfwSetErrorCallback(errorCallback);
    // inicia glfw
    if (!glfwInit())
    {
        depuracao::emitir(erro, "Iniciando window glfw");
        abort();
    }
    
    m_window = glfwCreateWindow(bounds.x, bounds.y, nome, NULL, NULL);
    m_nome = nome;
    if (!m_window) {
        depuracao::emitir(erro, "Janla invalida");
        abort();
    };

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        depuracao::emitir(erro, "Glad");
        abort();
    }
    

    if(icon_path)
    {
    imageLoader _icone(icon_path);
    auto glfw_icone = _icone.converterParaGlfw();
    glfwSetWindowIcon(m_window, 1, &glfw_icone);
    }
    // ativa blend
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_FRAMEBUFFER_SRGB);    

    glfwSetCursorPosCallback(m_window,mousePosCallback);
    glfwSetCharCallback(m_window, charCallback);
    glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
    glfwSetKeyCallback(m_window,keyCallback);
    glfwSetWindowUserPointer(m_window, this);

    vetor4<int> tam{};
    glfwGetWindowSize(m_window, &tam.z, &tam.w);
    tamanho.y = tam.w;
    tamanho.x = tam.z;
}

window::window(const char* nome, const bool f, fvet2 bounds , const char* icon_path)
{
    glfwSetErrorCallback(errorCallback);
    // inicia glfw
    if (!glfwInit())
    {
        depuracao::emitir(erro, "Iniciando window glfw");
        abort();
    }
if(f)
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    m_window = glfwCreateWindow(bounds.x, bounds.y, nome, NULL, NULL);
    m_nome = nome;
    if (!m_window) {
        depuracao::emitir(erro, "Janla invalida");
        abort();
    };

    glfwMakeContextCurrent(m_window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        depuracao::emitir(erro, "Glad");
        abort();
    }

    if(icon_path) {
        imageLoader _icone(icon_path);
        auto glfw_icone = _icone.converterParaGlfw();
        glfwSetWindowIcon(m_window, 1, &glfw_icone);
    }
    // ativa blend
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glfwSetCursorPosCallback(m_window,mousePosCallback);
    glfwSetCharCallback(m_window, charCallback);
    glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
    glfwSetKeyCallback(m_window,keyCallback);
    glfwSetWindowUserPointer(m_window, this);

    glfwGetWindowSize(m_window, &tamanho.x, &tamanho.y);
}
void window::poll()
{
    m_inputs.m_backspace_pressionado = false;
    m_inputs.m_backspace_repetido = false;

    glfwPollEvents();
    glfwGetWindowSize(m_window, &tamanho.x, &tamanho.y);
}

void window::swap() 
{
    if(m_cursor != m_cursor_antigo) 
    {
        auto cursor_glfw = glfwCreateStandardCursor((int)m_cursor);
        if(cursor_glfw)
        glfwSetCursor(m_window, cursor_glfw);
        m_cursor_antigo = m_cursor;
    }
    m_tempo.calcularDT();
    m_inputs.m_letra_pressionada = false;
    glfwSwapBuffers(m_window);
}

void window::viewport() const
{
    glViewport(0, 0, tamanho.x, tamanho.y);
}

void window::nome(const char* novo_nome)
{
    glfwSetWindowTitle(m_window, novo_nome);
    m_nome = novo_nome;
}

std::string window::nome() const
{
    return m_nome;
}

void window::modoLegado() {
// TODO modo legado / legacy mode
}

void window::loop() {
    while(!glfwWindowShouldClose(m_window)) {
        poll();
        swap();
    }
}
