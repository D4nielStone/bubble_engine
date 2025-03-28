#include "sistemas/bubble_gui.hpp"
#include "os/janela.hpp"
#include "nucleo/projeto.hpp"
#include "depuracao/debug.hpp"
#include "filesystem"
#include "util/runtime.hpp"
#include "sistemas/editor.hpp"

using namespace bubble;

void configurarInterface(bubble::projeto& proj)
{
    bubble_gui* gui = static_cast<bubble_gui*>(proj.obterSistema("bubble_gui"));
    sistema_editor* se = static_cast<sistema_editor*>(proj.obterSistema("editor"));
    // head
    if(!gui) {
        depuracao::emitir(erro, "interface", "sistema de gui inválido");
        return;
    }
    gui->adiFlags("raiz", flags_caixa::modular);
    gui->obterElemento("raiz")->m_orientacao_modular = caixa::orientacao::vertical;

    // menu
    gui->novoEstilo();
        gui->adiElemento<caixa>("raiz", "menu");
        gui->defFlags       (flags_caixa::modular | flags_caixa::largura_percentual);
        gui->defAltura      (                          35.f);
        gui->defLargura      (                           1.f);
        gui->defOrientacao  (   caixa::orientacao::horizontal);
        gui->defPaddingG    (                      5.f, 5.f);
        gui->defCorFundo    (    cor(0.f, 0.f, 0.f, 1.f));
    
    gui->novoEstilo();
        gui->adiElemento<caixa>("raiz", "raiz_b");
        gui->defFlags       (flags_caixa::modular | flags_caixa::largura_percentual);
        gui->defLargura      (                           1.f);
        gui->defOrientacao  (   caixa::orientacao::horizontal);
        gui->defCorFundo    (    cor(0.f, 0.f, 0.f, 0.f));
        gui->defCrescimentoM(                           1.f);
    // entidades
    gui->novoEstilo();
        gui->adiElemento<caixa>("raiz_b", "entidades");
        
        gui->defFlags       (flags_caixa::altura_percentual | flags_caixa::modular);
        gui->defLargura     (                          35.f);
        gui->defAltura      (                           1.f);
        gui->defOrientacao  (   caixa::orientacao::vertical);
        gui->defPaddingG    (                      5.f, 5.f);
        gui->defCorFundo    (    cor(0.1f, 0.1f, 0.1f, 1.f));

    gui->novoEstilo();
        
        for(auto& [entidade, comp] : proj.obterFaseAtual()->obterRegistro()->entidades)
        {
            std::string icone = "cube.png";
            if(comp.find(componente::COMPONENTE_LUZ_DIRECIONAL) != comp.end()) icone =  "Iluminacao.png";
            if(comp.find(componente::COMPONENTE_RENDER) != comp.end())icone =           "Renderizador.png";
            if(comp.find(componente::COMPONENTE_TRANSFORMACAO) != comp.end())icone =    "Transformacao.png";
            if(comp.find(componente::COMPONENTE_CODIGO) != comp.end())icone =           "Codigo.png";
            if(comp.find(componente::COMPONENTE_CAM) != comp.end())icone =              "Camera.png";
            gui->adiElemento<elementos::imagem>("entidades", "entidade " + std::to_string(entidade), icone);
        }

        gui->defLargura     (                          25.f);
        gui->defAltura      (                          25.f);
        gui->defCorFundo    (    cor(0.0f, 0.0f, 0.0f, 0.f));

    // editor
    gui->novoEstilo();
        gui->adiElemento<elementos::imagem>("raiz_b", "imagem_editor", se->cam.textura, true);
        
        gui->defFlags       (flags_caixa::altura_percentual | flags_caixa::modular);
        gui->defAltura      (                           1.f);
        gui->defOrientacao  ( caixa::orientacao::horizontal);
        gui->defCorFundo    (    cor(0.0f, 0.0f, 0.0f, 0.f));
        gui->defCrescimentoM(                           1.f);
    // define ponteiro viewport
        se->cam.viewport_ptr = &static_cast<elementos::imagem*>(gui->obterElemento("imagem_editor"))->m_imagem_tamanho;

    // botão de play
    gui->novoEstilo();
        gui->adiElemento<elementos::botao>("imagem_editor", "btn_play", "Play.png", sistema_editor::executarRuntime);
        gui->defPadding         (15.f, 15.f);
        gui->defLargura         (        30);
        gui->defAltura          (        30);
        gui->defCorFundo        (cor(0.0f, 0.0f, 0.0f, 0.0f));
    gui->novoEstilo();
        gui->adiElemento<caixa>("raiz_b", "componentes");
        gui->defFlags       (flags_caixa::altura_percentual | flags_caixa::modular);
        gui->defAltura      (                           1.f);
        gui->defCrescimentoM(                          0.6f);
        gui->defOrientacao  ( caixa::orientacao::vertical);
        gui->defCorFundo    (    cor(0.1f, 0.1f, 0.1f, 1.f));
}
void ini(const std::string& DIR_PADRAO)
{
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    // Cria projeto
    bubble::projeto editor(DIR_PADRAO);
   
    // Camera editor
    bubble::sistema_editor s_e;
    s_e.cam.ativarFB(); // Ativa framebuffer
    editor.adicionar("editor", &s_e);
    editor.srender()->definirCamera(&s_e.cam);

    // Adiciona sistema de GUI
    bubble::bubble_gui gui;
   
    // Adiciona sistema de gui ao projeto
    editor.adicionar("bubble_gui", &gui);

    /*  Config da interface   */
    configurarInterface(editor);
    /*                        */

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
