#include <stdexcept>
#include "commons_namespace.hpp"
#include "glad.h"
#include "GLFW/glfw3.h"
#include "os/window.hpp"
#include "debugging/debug.hpp"
#include "loaders/image_loader.hpp"
#include "loaders/shader.hpp"
#include "loaders/font.hpp"
#include "inputs/inputs.hpp"
using namespace COMMONS_NS;

void errorCallback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

bool window::hasInstance() {
    return instanceAtual;
}

window& window::get_instance() {
    if (!hasInstance())
       throw std::runtime_error("Instância da window não foi gerada!");
    return *instanceAtual;
}
window& window::newInstance(const char* nome, fvector_type2 bounds , const char* icon_path ) {
    if(instanceAtual) delete instanceAtual;
    instanceAtual = new window(nome, bounds, icon_path);
    return *instanceAtual;
}
window& window::newInstance(const char* nome, const bool f, fvector_type2 bounds , const char* icon_path ) {
    if(instanceAtual) delete instanceAtual;
    instanceAtual = new window(nome, f, bounds, icon_path);
    return *instanceAtual;
}

ivector_type2 window::get_window_size() {
    return window::get_instance().size;
};

void window::posicionarCursor(double x, double y)
{
    auto& input = window::get_instance().m_inputs;

    input.m_mousex = x;
    input.m_mousey = y;
    glfwSetCursorPos(window::get_instance().m_window, x, y);
}

window::~window() {
    desload_shaders();
    font_manager::limparFontes();
}
window::window(const char* nome, fvector_type2 bounds, const char* icon_path)
{
    glfwSetErrorCallback(errorCallback);
    // inicia glfw
    if (!glfwInit())
    {
        debugging::emit(erro, "Iniciando window glfw");
        abort();
    }

    m_window = glfwCreateWindow(bounds.x, bounds.y, nome, NULL, NULL);
    m_nome = nome;
    if (!m_window) {
        debugging::emit(erro, "Janla invalida");
        abort();
    };

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        debugging::emit(erro, "Glad");
        abort();
    }


    if(icon_path)
    {
    image_loader _icone(icon_path);
    auto glfw_icone = _icone.convert_to_glfw();
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

    vector4<int> tam{};
    glfwGetWindowSize(m_window, &tam.z, &tam.w);
    size.y = tam.w;
    size.x = tam.z;
}

window::window(const char* nome, const bool f, fvector_type2 bounds , const char* icon_path)
{
    glfwSetErrorCallback(errorCallback);
    // inicia glfw
    if (!glfwInit())
    {
        debugging::emit(erro, "Iniciando window glfw");
        abort();
    }
if(f)
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    m_window = glfwCreateWindow(bounds.x, bounds.y, nome, NULL, NULL);
    m_nome = nome;
    if (!m_window) {
        debugging::emit(erro, "Janla invalida");
        abort();
    };

    glfwMakeContextCurrent(m_window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        debugging::emit(erro, "Glad");
        abort();
    }

    if(icon_path) {
        image_loader _icone(icon_path);
        auto glfw_icone = _icone.convert_to_glfw();
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

    glfwGetWindowSize(m_window, &size.x, &size.y);
}
void window::poll()
{
    m_inputs.m_backspace_pressionado = false;
    m_inputs.m_backspace_repetido = false;

    glfwPollEvents();
    glfwGetWindowSize(m_window, &size.x, &size.y);
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
    m_time.calculateDT();
    m_inputs.m_letra_pressionada = false;
    glfwSwapBuffers(m_window);
}

void window::viewport() const
{
    glViewport(0, 0, size.x, size.y);
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
