#include "sistemas/bubble_gui.hpp"
#include "sistemas/editor.hpp"
#include "util/runtime.hpp"
#include "os/janela.hpp"
#include "nucleo/projeto.hpp"

using namespace bubble;
bool gatilho_ = true;
void sistema_editor::configurarInterface(bubble::projeto& proj)
{
    bubble_gui* gui = static_cast<bubble_gui*>(proj.obterSistema("bubble_gui"));
    // head
    if(!gui) {
        depuracao::emitir(erro, "interface", "sistema de gui inválido");
        return;
    }

    gui->adicionarFlags("raiz", flags_caixa::modular);
    gui->obterElemento("raiz")->m_orientacao_modular = caixa::orientacao::vertical;

    gui->obterElemento("raiz")->m_cor_fundo = cor(0.15f, 0.15f, 0.15f, 1.f);
    // menu
    gui->novoEstilo();
        gui->adicionarElemento<caixa>("raiz", "menu");
        gui->defFlags       (flags_caixa::modular);
        gui->defAltura      (                            30);
        gui->defLargura     (                           1.0);
        gui->defOrientacao  (   caixa::orientacao::horizontal);
        gui->defPaddingG    (                      5, 5);
        gui->defCorFundo    (    cor(0.15f, 0.15f, 0.15f, 1.f));
    gui->novoEstilo();
        gui->adicionarElemento<caixa>("raiz", "raiz_b");
        gui->defFlags       (flags_caixa::modular);
        gui->defLargura     (                           1.0);
        gui->defOrientacao  (   caixa::orientacao::horizontal);
        gui->defCorFundo    (    cor(0.f, 0.f, 1.f, 0.f));
        gui->defCrescimentoM(                            1.0f);
    gui->novoEstilo();
        gui->adicionarElemento<caixa>("raiz", "raiz_c");
        gui->defFlags       (flags_caixa::modular);
        gui->defLargura     (                           1.0);
        gui->defOrientacao  (   caixa::orientacao::horizontal);
        gui->defCorFundo    (    cor(0.05f, 0.05f, 0.05f, 1.f));
        gui->defCrescimentoM(                            0.2f);
    gui->novoEstilo();
        gui->adicionarElemento<elementos::texto>("raiz_c", "console", depuracao::obterMensagens(), 0.65f);
        gui->defAltura(1.0);
        // items menu
    gui->novoEstilo();
        gui->adicionarElemento<elementos::botao>("menu", "rc", [gui](){
                if(gui->obterElemento("raiz_c")->m_crescimento_modular == 0)
                {
                    
                gui->obterElemento("raiz_c")->m_crescimento_modular = 0.2;
                gui->obterElemento("raiz_c")->m_ativo = true;
                }
                else
                {
                gui->obterElemento("raiz_c")->m_crescimento_modular = 0;
                gui->obterElemento("raiz_c")->m_ativo = false;
                }}, "Mostrar Console");
        gui->obterElemento("raiz_c")->m_ativo = false;
        gui->obterElemento("raiz_c")->m_crescimento_modular = 0;
        gui->adicionarElemento<elementos::botao>("menu", "arquivo", [](){}, "Arquivo");
        gui->adicionarElemento<elementos::botao>("menu", "editar", [](){}, "Editar");
        gui->adicionarElemento<elementos::botao>("menu", "visualizar", [](){}, "Exibir");
        gui->adicionarElemento<elementos::botao>("menu", "ajuda", [](){}, "Ajuda");
        gui->adicionarElemento<elementos::botao>("menu", "add_primitivas", [](){}, new elementos::imagem("cubo_branco"));
        gui->defCorBorda    (cor(1.f, 1.f, 1.f, 1.f));
        gui->defCorFundo    (cor(0.15f, 0.15f, 0.15f, 1.f));
        gui->defPaddingG    (5, 5);
        gui->defAltura(22);
        gui->defLargura(22);
    // entidades
    gui->novoEstilo();
        gui->adicionarElemento<caixa>("raiz_b", "entidades");
        
        gui->defFlags       (flags_caixa::modular);
        gui->defLargura     (                          35);
        gui->defAltura      (                           1.0);
        gui->defOrientacao  (   caixa::orientacao::vertical);
        gui->defPaddingG    (                      5, 5);
        gui->defCorFundo    (    cor(0.1f, 0.1f, 0.1f, 1.f));

    // editor
    gui->novoEstilo();
        gui->adicionarElemento<elementos::imagem>("raiz_b", "imagem_editor", cam.textura, true);
        
        gui->defFlags       (flags_caixa::altura_percentual | flags_caixa::modular);
        gui->defAltura      (                           1.0);
        gui->defOrientacao  ( caixa::orientacao::horizontal);
        gui->defCorFundo    (    cor(0.0f, 0.0f, 0.0f, 0.f));
        gui->defCorBorda    (    cor(0.07f, 0.07f, 0.07f, 1.f));
        gui->defCrescimentoM(                           1.f);
        gui->defTamanhoBorda(                           4);
    // define ponteiro viewport
        cam.viewport_ptr = &static_cast<elementos::imagem*>(gui->obterElemento("imagem_editor"))->m_imagem_tamanho;
    gui->novoEstilo();
        gui->adicionarElemento<elementos::botao>("imagem_editor", "btn_play", [](){
                    projeto_atual->salvarTudo();
                    sistema_editor::executarRuntime();
                }, new elementos::imagem("Play.png"));
        gui->defPadding      (15, 15);
        gui->defLargura      (        30);
        gui->defAltura       (        30);
        gui->defCorFundo     (cor(0.0f, 0.0f, 0.0f, 0.0f));
    gui->novoEstilo();
        gui->adicionarElemento<caixa>("raiz_b", "componentes");
        gui->defFlags        (flags_caixa::altura_percentual | flags_caixa::modular);
        gui->defAltura       (                           1.0);
        gui->defCrescimentoM (0.5);
        gui->defOrientacao   ( caixa::orientacao::vertical);
        gui->defCorFundo     (    cor(0.1f, 0.1f, 0.1f, 1.f));
    // texto
    gui->novoEstilo();
        gui->adicionarElemento<elementos::texto>("componentes", "texto2", &texto_entidade, 0.7f, elementos::flags_texto::alinhamento_central);
        gui->defCorFundo     (cor(0.07f, 0.07f, 0.07f, 1.f));
        gui->defFlags        (flags_caixa::largura_percentual );
        gui->defLargura      (1.0);
        gui->defAltura       (17);
    gui->novoEstilo();
        gui->adicionarElemento<caixa>("componentes", "area_comps");
        gui->defCorFundo     (    cor(0.08f, 0.08f, 0.08f, 1.f));
        gui->defFlags        (flags_caixa::largura_percentual | flags_caixa::modular);
        gui->defLargura       (                           1.0);
        gui->defAltura       (                           35);
        gui->defPaddingG     (                           5, 5);
        gui->defOrientacao   ( caixa::orientacao::horizontal);
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
    atualizarEntidades();
    atualizarComponentes();
    /*                        */
}

void sistema_editor::atualizar()
{
    // inputs
    if(instanciaJanela->inputs.isKeyPressed("F5"))
    {
        projeto_atual->salvarTudo();
        executarRuntime();
    }
    // Verifica se o número de entidades mudou
    size_t num_entidades_atual = projeto_atual->obterFaseAtual()->obterRegistro()->entidades.size();
    if (num_entidades_atual != num_entidades_anterior)
    {
        atualizarEntidades(); // Atualiza a interface de entidades
        num_entidades_anterior = num_entidades_atual; // Atualiza a referência
    }
    // Verifica se a entidade atual mudou
    if (entidade_anterior != entidade_atual)
    {
        atualizarComponentes(); 
        entidade_anterior = entidade_atual;
    }
    // camera
    if(instanciaJanela->inputs.isKeyPressed("Shif"))
    {
        if(gatilho_ && instanciaJanela->inputs.isKeyPressed("A"))
        {
        projeto_atual->obterFaseAtual()->obterRegistro()->criar();
        gatilho_ = false;
        }else if(gatilho_ && instanciaJanela->inputs.isKeyPressed("X"))
        {
projeto_atual->obterFaseAtual()->obterRegistro()->remover(entidade_atual);
        gatilho_ = false;
        }
        else if(!instanciaJanela->inputs.isKeyPressed("X") && !instanciaJanela->inputs.isKeyPressed("A"))
        {
        gatilho_ = true;
        }
    }
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
void sistema_editor::atualizarEntidades()
{
    bubble_gui* gui = static_cast<bubble_gui*>(projeto_atual->obterSistema("bubble_gui"));
    // head
    if(!gui) {
        depuracao::emitir(erro, "interface", "sistema de gui inválido");
        return;
    }

    // Remove os botões antigos para evitar duplicatas
    gui->removerFilhos("entidades");
    
    if(!projeto_atual->obterFaseAtual()->obterRegistro()->entidades.empty())
    {
    gui->novoEstilo();
    // Adiciona os botões para cada entidade existente
    for (auto& [id, comp] : projeto_atual->obterFaseAtual()->obterRegistro()->entidades)
    {
        std::string icone = "cube.png";
        gui->adicionarElemento<elementos::botao>("entidades",
            std::to_string(id),
            [id, this]()
            {
                entidade_atual = id;
                texto_entidade = "id:" + std::to_string(entidade_atual);
            },
            new elementos::imagem(icone));
                entidade_atual = id;
                texto_entidade = "id:" + std::to_string(entidade_atual);
    }
        gui->defLargura     (                          25);
        gui->defAltura      (                          25);
        gui->defCorFundo    (    cor(0.0f, 0.0f, 0.0f, 0.f));
    } else 
    {
        entidade_atual = 0;
        texto_entidade = "Nenhuma entidade selecionada";
    }
    atualizarComponentes();
}
void sistema_editor::atualizarComponentes()
{
    bubble_gui* gui = static_cast<bubble_gui*>(projeto_atual->obterSistema("bubble_gui"));
    if (!gui) return;

    // Remove os botões antigos para evitar duplicatas
    gui->removerFilhos("area_comps");
    if(entidade_atual == 0) return;
    gui->novoEstilo();
    size_t i = 0;
    for (auto& [mascara, componente] : projeto_atual->obterFaseAtual()->obterRegistro()->entidades[entidade_atual]) 
    {
        i++;
        std::string icone = "cube.png";
            if(mascara == componente::COMPONENTE_LUZ_DIRECIONAL) icone =  "Iluminacao.png";
            if(mascara == componente::COMPONENTE_LUZ_PONTUAL) icone =  "Iluminacao.png";
            if(mascara == componente::COMPONENTE_RENDER) icone =           "Renderizador.png";
            if(mascara == componente::COMPONENTE_TRANSFORMACAO) icone =    "Transformacao.png";
            if(mascara == componente::COMPONENTE_CODIGO) icone =           "Codigo.png";
            if(mascara == componente::COMPONENTE_CAM) icone =              "Camera.png";
            if(mascara == componente::COMPONENTE_TERRENO) icone =              "Terreno.png";
        gui->adicionarElemento<elementos::botao>("area_comps",
            icone.erase(icone.size() - 3),
            [](){},
            new elementos::imagem(icone));
    }
        gui->defLargura     (                          25);
        gui->defAltura      (                          25);
        gui->defCorFundo    (    cor(0.0f, 0.0f, 0.0f, 0.f));
}
