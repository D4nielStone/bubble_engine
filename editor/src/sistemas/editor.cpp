#include "sistemas/bubble_gui.hpp"
#include "sistemas/editor.hpp"
#include "util/runtime.hpp"
#include "os/janela.hpp"
#include "nucleo/projeto.hpp"

using namespace bubble;
std::string numero_entidade = "";
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
        gui->defFlags       (flags_caixa::modular | flags_caixa::largura_percentual);
        gui->defAltura      (                          32.f);
        gui->defLargura     (                           1.f);
        gui->defOrientacao  (   caixa::orientacao::horizontal);
        gui->defPaddingG    (                      5.f, 5.f);
        gui->defCorFundo    (    cor(0.15f, 0.15f, 0.15f, 1.f));
    gui->novoEstilo();
        gui->adiElemento<elementos::botao>("menu", "arquivo", [](){}, "Arquivo");
        gui->adiElemento<elementos::botao>("menu", "editar", [](){}, "Editor");
        gui->adiElemento<elementos::botao>("menu", "visualizar", [](){}, "Visualizar");
        gui->adiElemento<elementos::botao>("menu", "ajuda", [](){}, "Ajuda");
        gui->defCorFundo    (    cor(0.2f, 0.2f, 0.2f, 1.f));
        gui->defPaddingG       (5.f, 5.f);
    gui->novoEstilo();
        gui->adiElemento<caixa>("raiz", "raiz_b");
        gui->defFlags       (flags_caixa::modular | flags_caixa::largura_percentual);
        gui->defLargura      (                           1.f);
        gui->defOrientacao  (   caixa::orientacao::horizontal);
        gui->defCorFundo    (    cor(0.f, 0.f, 1.f, 0.f));
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
            gui->adiElemento<elementos::botao>("entidades",
                    std::to_string(entidade),
                    [entidade, numero_entidade](){numero_entidade = "entidade " + std::to_string(entidade);},
                    new elementos::imagem(icone));
        }

        gui->defLargura     (                          25.f);
        gui->defAltura      (                          25.f);
        gui->defCorFundo    (    cor(0.0f, 0.0f, 0.0f, 0.f));

    // editor
    gui->novoEstilo();
        gui->adiElemento<elementos::imagem>("raiz_b", "imagem_editor", cam.textura, true);
        
        gui->defFlags       (flags_caixa::altura_percentual | flags_caixa::modular);
        gui->defAltura      (                           1.f);
        gui->defOrientacao  ( caixa::orientacao::horizontal);
        gui->defCorFundo    (    cor(0.0f, 0.0f, 0.0f, 0.f));
        gui->defCorBorda    (   cor(0.98f, 0.76f, 0.09f, 1.f));
        gui->defCrescimentoM(                           1.f);
        gui->defTamanhoBorda(                           5.f);
    // define ponteiro viewport
        cam.viewport_ptr = &static_cast<elementos::imagem*>(gui->obterElemento("imagem_editor"))->m_imagem_tamanho;
    gui->novoEstilo();
        gui->adiElemento<elementos::botao>("imagem_editor", "btn_play", sistema_editor::executarRuntime, new elementos::imagem("Play.png"));
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
    // texto
    gui->novoEstilo();
        gui->adiElemento<elementos::texto>("componentes", "texto1", "Componentes", 0.7f, elementos::flags_texto::alinhamento_central);
        gui->adiElemento<elementos::texto>("componentes", "texto2", &numero_entidade, 0.7f, elementos::flags_texto::alinhamento_central);
        gui->defCorFundo    (   cor(0.98f, 0.76f, 0.09f, 1.f));
        gui->defFlags       (flags_caixa::largura_percentual );
        gui->defLargura         (1.f);
        gui->defAltura         (16.8f);
    // botão de play
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
