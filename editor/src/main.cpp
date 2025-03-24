#include "os/janela.hpp"
#include "nucleo/projeto.hpp"
#include "os/sistema.hpp"
#include "depuracao/debug.hpp"
#include "filesystem"
#include "componentes/camera_editor.hpp"
#include "util/runtime.hpp"
//#include "ui/bubble_gui.hpp"

struct sistema_cam_editor : public bubble::sistema
{
        sistema_cam_editor()
        {
        }
        void atualizar() override
        {
            cam.atualizarMovimentacao();
        }
        bubble::camera_editor cam;
};
void ini(const std::string& DIR_PADRAO)
{
    // Cria projeto
    bubble::projeto editor(DIR_PADRAO);
   
    // TODO: sistema de parsing e interface
    
    // Camera editor
    sistema_cam_editor scam_e;
    scam_e.cam.viewport_ptr = &bubble::instanciaJanela->tamanho;
    editor.adicionar(&scam_e);
    editor.srender()->definirCamera(&scam_e.cam);

    /*
    // Adiciona sistema de GUI
    bgui::bubble_gui gui;
    
    // Adiciona ancora à ancora principal
    auto aI = gui.adicionar<bgui::ancora>(bgui::ancora_principal, bgui::ancora::horizontal);
    gui.adicionar<imagem>(aI, camera_editor->textura);

    editor.adicionar(&gui);
    */
    // Defini o nome da janela
    bubble::instanciaJanela->nome(
            (std::string("editor (c) Bubble 2025 :: Projeto ") 
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
