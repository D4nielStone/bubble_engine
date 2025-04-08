#include "sistemas/editor.hpp"
#include "sistemas/bubble_gui.hpp"
#include "os/janela.hpp"
#include "nucleo/projeto.hpp"
#include "depuracao/debug.hpp"
#include "filesystem"
#include "util/runtime.hpp"
#include <GLFW/glfw3.h>
#include "arquivadores/shader.hpp"

using namespace bubble;

void ini(const std::string& DIR_PADRAO)
{

    if(!std::filesystem::exists(DIR_PADRAO))
        {
            depuracao::emitir(alerta, "Diretório do projeto inexistente.");
            if(!std::filesystem::create_directories(DIR_PADRAO))
            {
                depuracao::emitir(erro, "criando dir padrao.");
                return;
            }
        }
    bubble::instanciaJanela = new bubble::janela("gerenciador de projetos | Daniel O. dos Santos© Bubble 2025", true);
   
    // Sistema de gui
    bubble_gui gui;
    gui.inicializar(nullptr);
    gui.obterElemento("raiz")->m_cor_fundo = {0.18f, 0.18f, 0.18f, 1.f};
    gui.adicionarFlags("raiz", flags_caixa::modular);
    
    gui.novoEstilo();
    gui.adicionarElemento<caixa>("raiz", "barra_lateral");
        gui.defFlags(flags_caixa::modular | flags_caixa::largura_justa);
        gui.defOrientacao(caixa::orientacao::vertical);
        gui.defCorFundo({0.1, 0.1, 0.1, 1});
        gui.defPaddingG(5, 5);
        gui.defAltura(1.0);

    gui.novoEstilo();
        gui.adicionarElemento<elementos::texto>("barra_lateral", "texto1", "Projetos encontrados", 20);
    gui.novoEstilo();
        for (const auto& entry : std::filesystem::recursive_directory_iterator(DIR_PADRAO)) {
            if (entry.is_regular_file() && entry.path().filename() == "config.json") {
                auto caminhoEncontrado = entry.path().parent_path().string();
                gui.novoEstilo();    
                gui.adicionarElemento<elementos::botao>("barra_lateral", "abrirProjeto", [caminhoEncontrado](){
        
                glfwDestroyWindow(instanciaJanela->window);
                delete instanciaJanela;
                instanciaJanela = nullptr;
                bubble::descarregarShaders();
                // Cria projeto
                bubble::projeto editor(caminhoEncontrado);
               
                // Sistema do editor
                editor.adicionar("editor", new sistema_editor());
            
            
                // Define o nome da janela
                bubble::instanciaJanela->nome(
                        (std::string("editor | Daniel O. dos Santos© Bubble 2025 | ") 
                         + bubble::instanciaJanela->nome()).c_str());
            
                // Inicia main loop
                editor.rodar();
        
                }, new elementos::imagem("folder.png"));
                gui.defLargura(40);
                gui.defAltura(40);
                gui.novoEstilo();    
                gui.adicionarElemento<elementos::texto>("barra_lateral", caminhoEncontrado, caminhoEncontrado);
            }
        }

    gui.novoEstilo();
        gui.adicionarElemento<caixa>("raiz", "area_maior");
        gui.defFlags(flags_caixa::modular | flags_caixa::alinhamento_central);
        gui.defAltura(1.0);
        gui.defCrescimentoM(1.0);
    gui.novoEstilo();
        gui.adicionarElemento<elementos::botao>("area_maior", "add", [](){}, new elementos::texto("+", 48));
    while(!glfwWindowShouldClose(instanciaJanela->window))
    {
        instanciaJanela->poll();
        
        gui.atualizar();

        instanciaJanela->swap();
    }
}
int main(int argc, char* argv[]) {
    // Definir diretório do projeto
    std::string DIR_PADRAO = std::string(std::getenv("HOME")) + "/bubble/jogos";

    if (argc > 1) {
        DIR_PADRAO = argv[1];
    }

    try {
            ini(DIR_PADRAO);
        }
    catch (const std::exception& e) {
        depuracao::emitir(erro, e.what());
    }

    return 0;
}
