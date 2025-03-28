#include "sistemas/bubble_gui.hpp"
#include "os/janela.hpp"
#include "nucleo/projeto.hpp"
#include "depuracao/debug.hpp"
#include "filesystem"
#include "util/runtime.hpp"
#include "sistemas/editor.hpp"

using namespace bubble;

void configurarInterface(bubble::bubble_gui* gui, bubble::sistema_editor* se)
{
    // head
    gui->adiFlags("raiz", flags_caixa::modular);

    // menu
    gui->novoEstilo();
        gui->adiElemento<caixa>("raiz", "menu");
        
        gui->defFlags       (flags_caixa::altura_percentual | flags_caixa::modular);
        gui->defLargura     (                          35.f);
        gui->defAltura      (                           1.f);
        gui->defOrientacao  (   caixa::orientacao::vertical);
        gui->defPaddingG    (                      5.f, 5.f);
        gui->defCorFundo    (    cor(0.1f, 0.1f, 0.1f, 1.f));

    gui->novoEstilo();
        gui->adiElemento<elementos::imagem>("menu", "btn1", "cube.png", true);
        gui->adiElemento<elementos::imagem>("menu", "btn2", "cube.png", true);
        gui->adiElemento<elementos::imagem>("menu", "btn3", "cube.png", true);
    
        gui->defLargura     (                          25.f);
        gui->defAltura      (                          25.f);
        gui->defCorFundo    (    cor(0.0f, 0.0f, 0.0f, 0.f));

    // editor
    gui->novoEstilo();
        gui->adiElemento<elementos::imagem>("raiz", "imagem_editor", se->cam.textura);
        
        gui->defFlags       (flags_caixa::altura_percentual | flags_caixa::modular);
        gui->defAltura      (                           1.f);
        gui->defOrientacao  ( caixa::orientacao::horizontal);
        gui->defCorFundo    (    cor(0.0f, 0.0f, 0.0f, 0.f));
        gui->defCrescimentoM(                           1.f);
    // define ponteiro viewport
        se->cam.viewport_ptr = &static_cast<elementos::imagem*>(gui->obterElemento("imagem_editor"))->m_imagem_tamanho;

    // botão de play
    gui->novoEstilo();
        gui->adiElemento<elementos::imagem>("imagem_editor", "btn_play", "Play.png");
        gui->defPadding         (15.f, 15.f);
        gui->defLargura         (        30);
        gui->defAltura          (        30);
        gui->defCorFundo        (cor(0.0f, 0.0f, 0.0f, 0.0f));
}
void ini(const std::string& DIR_PADRAO)
{
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    // Cria projeto
    bubble::projeto editor(DIR_PADRAO);
   
    // Camera editor
    bubble::sistema_editor s_e;
    s_e.cam.ativarFB(); // Ativa framebuffer
    editor.adicionar(&s_e);
    editor.srender()->definirCamera(&s_e.cam);

    // Adiciona sistema de GUI
    bubble::bubble_gui gui;
   
    /*  Config da interface   */
    configurarInterface(&gui, &s_e);
    /*                        */

    // Adiciona sistema de gui ao projeto
    editor.adicionar(&gui);

    // Define o nome da janela
    bubble::instanciaJanela->nome(
            (std::string("editor (c) Bubble 2025 | ") 
             + bubble::instanciaJanela->nome()).c_str());

    // Inicia main loop
    editor.rodar();
}

int main(int argc, char* argv[]) {
    // Definir diretório do projeto
    std::string DIR_PADRAO = std::string(std::getenv("HOME")) + "/bubble/jogos";

    if (argc > 1) {
        DIR_PADRAO = argv[1];
    }

    // Cria diretório inexistente
    if(!std::filesystem::exists(DIR_PADRAO))
    {
        depuracao::emitir(alerta, "Diretório inexistente. criar diretório? S(sim) N(não)");
    
    std::string resp;
    std::cin >> resp;
    if(resp == "S" || resp == "s")
        std::filesystem::create_directories(DIR_PADRAO);
    else
        return -1;
    }
    
    try {
            ini(DIR_PADRAO);
        }
    catch (const std::exception& e) {
        depuracao::emitir(erro, e.what());
    }

    return 0;
}
