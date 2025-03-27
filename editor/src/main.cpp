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
    gui->adicionarFlags("raiz", flags_caixa::modular);
    gui->obterElemento("raiz")->m_orientacao_modular = caixa::orientacao::vertical;

    // menu
    gui->adiElemento<caixa>("raiz", "menu");
    gui->adicionarFlags("menu", flags_caixa::largura_proporcional);
    gui->obterElemento("menu")->m_largura = 1.0;
    gui->obterElemento("menu")->m_altura = 30;

    // editor
    // adiciona imagem com buffer da camera do editor
    gui->adiElemento<elementos::imagem>("raiz", "imagem_editor", se->cam.textura);
    gui->adicionarFlags("imagem_editor", flags_caixa::largura_proporcional | flags_caixa::crescimento_vertical);
    gui->obterElemento("imagem_editor")->m_largura = 1.0;

    se->cam.viewport_ptr = &dynamic_cast<elementos::imagem*>(gui->obterElemento("imagem_editor"))->m_imagem_tamanho;

    gui->obterElemento("imagem_editor")->m_crescimento_modular = 1; // Ocupa todo o espaço disponível
    
    // menu (teste)
    gui->adiElemento<caixa>("raiz", "menu2");
    gui->adicionarFlags("menu2", flags_caixa::largura_proporcional);
    gui->obterElemento("menu2")->m_largura = 1.0;
    gui->obterElemento("menu2")->m_altura = 30;
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
