/** @copyright Copyright (c) 2025 Daniel Oliveira */

/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file gerenciador_projetos.cpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#include "sistemas/gerenciador_projetos.hpp"
#include <GLFW/glfw3.h>

using namespace bubble;

gerenciador_projetos::gerenciador_projetos(const std::string& DIR_PADRAO) : DIR_PADRAO(DIR_PADRAO)
{
}

void gerenciador_projetos::atualizarElementos(const std::string& Dir)
{
    // Barra Lateral (    Texto acima    )
    gui.novoEstilo();
        gui.adicionarElemento<elementos::texto>("barra_lateral", "texto1", "Projetos encontrados", 20, elementos::flags_texto::alinhamento_central);
        gui.defLargura(1.0);
    // Iteração pastas
    gui.novoEstilo();
        for (const auto& entry : std::filesystem::recursive_directory_iterator(Dir)) {
            if (entry.is_regular_file() && entry.path().filename() == "config.json") {
                auto caminhoEncontrado = entry.path().parent_path().string();
                gui.novoEstilo();    
                gui.adicionarElemento<elementos::botao>("barra_lateral", "abrirProjeto", 
                [caminhoEncontrado](){
        
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
        
                }
                , new elementos::imagem("folder.png"));
                gui.defLargura(40);
                gui.defAltura(40);
                gui.novoEstilo();    
                gui.adicionarElemento<elementos::texto>("barra_lateral", caminhoEncontrado, caminhoEncontrado);
            }
        }
}

void gerenciador_projetos::iniciar()
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
    gui.inicializar(nullptr);
    gui.obterElemento("raiz")->m_cor_fundo = {0.18f, 0.18f, 0.18f, 1.f};
    gui.adicionarFlags("raiz", flags_caixa::modular);
    
    // configura interface
    gui.novoEstilo();
    gui.adicionarElemento<caixa>("raiz", "barra_lateral");
        gui.defFlags(flags_caixa::modular | flags_caixa::largura_justa);
        gui.defOrientacao(caixa::orientacao::vertical);
        gui.defCorFundo({0.1, 0.1, 0.1, 1});
        gui.defPaddingG(5, 5);
        gui.defAltura(1.0);
    
    atualizarElementos(DIR_PADRAO);
    // Area Maior
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
