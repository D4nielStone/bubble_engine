#include <glad/glad.h>
#include "os/janela.hpp"
#include "depuracao/debug.hpp"
#include "arquivadores/imageloader.hpp"
// Callback de erro
void errorCallback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

bubble::janela::~janela()
{
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

void bubble::janela::poll() const
{

    glfwPollEvents();
}

void bubble::janela::swap() 
{
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    tamanho.y = h;
    tamanho.x = w;
    _Mtempo.calcularDT();
    glfwSwapBuffers(window);
}

void bubble::janela::viewport() const
{
    glViewport(0, 0, tamanho.x, tamanho.y);
}

void bubble::janela::nome(const char* novo_nome) const
{
    glfwSetWindowTitle(window, novo_nome);
}