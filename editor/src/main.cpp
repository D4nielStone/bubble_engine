#include "sistemas/bubble_gui.hpp"
#include "os/janela.hpp"
#include "nucleo/projeto.hpp"
#include "depuracao/debug.hpp"
#include "filesystem"
#include "util/runtime.hpp"
#include "sistemas/editor.hpp"
#include "util/caixas.hpp"

void ini(const std::string& DIR_PADRAO)
{
    // Cria projeto
    bubble::projeto editor(DIR_PADRAO);
   
    // TODO: sistema de parsing e interface
    
    // Camera editor
    bubble::sistema_editor s_e;
    s_e.cam.ativarFB(); // Ativa framebuffer
    editor.adicionar(&s_e);
    editor.srender()->definirCamera(&s_e.cam);

    // Adiciona sistema de GUI
    bubble::bubble_gui gui;
    
    // Adiciona ancora à ancora principal
    auto anc_p_div = gui.dividir(gui.ancora_principal, bubble::ancora::vertical); //< ancora a = menu
    auto ancoras_ab = gui.dividir(anc_p_div.second, bubble::ancora::horizontal);
    
    auto& divisao_a = gui.ancoras[ancoras_ab.second];
    
    gui.ancoras[anc_p_div.first]->tamanho = bubble::ancora::fixo; // menu fixo
    gui.ancoras[anc_p_div.first]->limites.w = 35; // tamanho menu
    
    // Adiciona caixa de editor
    divisao_a->corpo = bubble::caixa_editor(&s_e.cam);

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
