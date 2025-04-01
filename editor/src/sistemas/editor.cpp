#include "sistemas/bubble_gui.hpp"
#include "sistemas/editor.hpp"
#include "util/runtime.hpp"
#include "os/janela.hpp"
#include "nucleo/projeto.hpp"

using namespace bubble;
void sistema_editor::configurarInterface(bubble::projeto& proj)
{
    bubble_gui* gui = static_cast<bubble_gui*>(proj.obterSistema("bubble_gui"));
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
        gui->defFlags       (flags_caixa::modular);
        gui->defAltura      (                            32);
        gui->defLargura     (                           1.0);
        gui->defOrientacao  (   caixa::orientacao::horizontal);
        gui->defPaddingG    (                      5, 5);
        gui->defCorFundo    (    cor(0.15f, 0.15f, 0.15f, 1.f));
    // items menu
    gui->novoEstilo();
        gui->adiElemento<elementos::botao>("menu", "arquivo", [](){}, "Arquivo");
        gui->adiElemento<elementos::botao>("menu", "editar", [](){}, "Editar");
        gui->adiElemento<elementos::botao>("menu", "visualizar", [](){}, "Exibir");
        gui->adiElemento<elementos::botao>("menu", "ajuda", [](){}, "Ajuda");
        gui->defCorFundo    (cor(0.2f, 0.2f, 0.2f, 1.f));
        gui->defPaddingG    (5, 5);
    gui->novoEstilo();
        gui->adiElemento<caixa>("raiz", "raiz_b");
        gui->defFlags       (flags_caixa::modular);
        gui->defLargura     (                           1.0);
        gui->defOrientacao  (   caixa::orientacao::horizontal);
        gui->defCorFundo    (    cor(0.f, 0.f, 1.f, 0.f));
        gui->defCrescimentoM(                            1.0f);
    // entidades
    gui->novoEstilo();
        gui->adiElemento<caixa>("raiz_b", "entidades");
        
        gui->defFlags       (flags_caixa::modular);
        gui->defLargura     (                          35);
        gui->defAltura      (                           1.0);
        gui->defOrientacao  (   caixa::orientacao::vertical);
        gui->defPaddingG    (                      5, 5);
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
            gui->adiElemento<elementos::botao>("entidades",
                    std::to_string(entidade),
                    [entidade, this]()
                    {
                        entidade_atual = entidade;
                        texto_entidade = "entidade " + std::to_string(entidade_atual);
                    },
                    new elementos::imagem(icone));
        }

        gui->defLargura     (                          25);
        gui->defAltura      (                          25);
        gui->defCorFundo    (    cor(0.0f, 0.0f, 0.0f, 0.f));

    // editor
    gui->novoEstilo();
        gui->adiElemento<elementos::imagem>("raiz_b", "imagem_editor", cam.textura, true);
        
        gui->defFlags       (flags_caixa::altura_percentual | flags_caixa::modular);
        gui->defAltura      (                           1.0);
        gui->defOrientacao  ( caixa::orientacao::horizontal);
        gui->defCorFundo    (    cor(0.0f, 0.0f, 0.0f, 0.f));
        gui->defCorBorda    (   cor(0.98f, 0.76f, 0.09f, 1.f));
        gui->defCrescimentoM(                           1.f);
        gui->defTamanhoBorda(                           5);
    // define ponteiro viewport
        cam.viewport_ptr = &static_cast<elementos::imagem*>(gui->obterElemento("imagem_editor"))->m_imagem_tamanho;
    gui->novoEstilo();
        gui->adiElemento<elementos::botao>("imagem_editor", "btn_play", sistema_editor::executarRuntime, new elementos::imagem("Play.png"));
        gui->defPadding      (15, 15);
        gui->defLargura      (        30);
        gui->defAltura       (        30);
        gui->defCorFundo     (cor(0.0f, 0.0f, 0.0f, 0.0f));
    gui->novoEstilo();
        gui->adiElemento<caixa>("raiz_b", "componentes");
        gui->defFlags        (flags_caixa::altura_percentual | flags_caixa::modular);
        gui->defAltura       (                           1.0);
        gui->defCrescimentoM (0.5);
        gui->defOrientacao   ( caixa::orientacao::vertical);
        gui->defCorFundo     (    cor(0.1f, 0.1f, 0.1f, 1.f));
    // texto
    gui->novoEstilo();
        gui->adiElemento<elementos::texto>("componentes", "texto1", "Componentes", 0.7f, elementos::flags_texto::alinhamento_central);
        gui->adiElemento<elementos::texto>("componentes", "texto2", &texto_entidade, 0.7f, elementos::flags_texto::alinhamento_central);
        gui->defCorFundo     (cor(0.05f, 0.05f, 0.05f, 1.f));
        gui->defFlags        (flags_caixa::largura_percentual );
        gui->defLargura      (1.0);
        gui->defAltura       (17);


    // componentes
    gui->novoEstilo();
    int i = 0;
    if (!projeto_atual->obterFaseAtual()->obterRegistro()->entidades.empty())
        for (auto& componente : projeto_atual->obterFaseAtual()->obterRegistro()->entidades)
        {
            i++;
            gui->adiElemento<caixa>("componentes", std::to_string(i));
            gui->defLargura (1.0);
            gui->defAltura  (100);
            gui->defCorBorda(cor(0.15, 0.15, 0.15, 1));
            gui->defCorFundo(cor(0.2, 0.2, 0.2, 1));
        }
}

sistema_editor::sistema_editor()
{
}

void sistema_editor::inicializar(fase* f)
{
    sistema::inicializar(f);

    cam.ativarFB(); // Ativa framebuffer
    projeto_atual->srender()->definirCamera(&cam);
    
    // Adiciona sistema de gui ao projeto
    projeto_atual->adicionar("bubble_gui", new bubble_gui());
    /*  Config da interface   */
    configurarInterface(*projeto_atual);
    /*                        */
}

void sistema_editor::atualizar()
{
    // inputs
    if(instanciaJanela->inputs.isKeyPressed("F5"))
    {
        executarRuntime();
    }

    // camera
    cam.atualizarMovimentacao();
}

void sistema_editor::executarRuntime()
{
    // Inicia o runtime
    iniciarRuntime(); 

    // Se já houver uma thread rodando, não cria outra
    if (rodando.load()) return;

    // Marca como ativo
    rodando.store(true);

    // Inicia a thread de monitoramento
    threadRuntime = std::thread(&sistema_editor::monitorarRuntime);
    threadRuntime.detach(); // Libera a thread para rodar em segundo plano
}

void sistema_editor::monitorarRuntime()
{
    while (rodando.load())
    {
        if (!runtimeRodando()) // Verifica se o runtime ainda está ativo
        {
            rodando.store(false);
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
    }
}
