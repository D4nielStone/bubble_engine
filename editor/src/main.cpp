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
    gui->adicionarFlags("raiz", flags::modular);
    gui->obterCaixa("raiz")->m_orientacao_modular = caixa::orientacao::vertical;

    // menu
    gui->adicionarCaixa("raiz", "menu");
    gui->adicionarFlags("menu", flags::largura_proporcional);
    gui->obterCaixa("menu")->m_largura = 1.0;
    gui->obterCaixa("menu")->m_altura = 30;

    // editor
    gui->adicionarCaixa("raiz", "editor");
    gui->adicionarFlags("editor", flags::largura_proporcional | flags::crescimento_vertical);
    gui->obterCaixa("editor")->m_largura = 1.0;
    gui->obterCaixa("editor")->m_imagem_fundo = std::make_unique<imagem>(se->cam.textura);

    se->cam.viewport_ptr = &gui->obterCaixa("editor")->m_imagem_fundo->limite;

    gui->obterCaixa("editor")->m_crescimento_modular = 1; // Ocupa todo o espaço disponível
}

void ini(const std::string& DIR_PADRAO)
{
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
            (std::string("editor (c) Bubble 2025 :: Projeto ::--") 
             + bubble::instanciaJanela->nome() + "--::").c_str());

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
