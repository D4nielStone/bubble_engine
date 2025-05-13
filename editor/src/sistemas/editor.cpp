/** @copyright 
MIT License
Copyright (c) 2025 Daniel Oliveira

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
*/
/**
 * @file editor.cpp
 *
 * @author Daniel O. dos Santos
 *
 * @licence MIT License
 */

#include <cstdlib>
#include "util/versao.hpp"
#include "becommons/becommons.hpp"
#include "sistemas/editor.hpp"
#include "util/runtime.hpp"

using namespace becommons;
using namespace EDITOR_NS;

bool gatilho_ = true;

/// raiz
/// -- topo
/// -- -- barra menu
/// -- -- barra versao
/// -- centro
/// -- -- entidades
/// -- -- editor
/// -- -- componentes
/// -- console
/// -- -- output


sistema_editor::sistema_editor()
{
}
void sistema_editor::configurarInterface(projeto& proj)
{
    if(!gui) {
        depuracao::emitir(erro, "interface", "sistema de gui inválido");
        return;
    }
    /// raiz
    gui->iniciarRaiz        ("raiz");
        gui->defFlags       (           flag_estilo::modular);
        gui->defOrientacao  (   estilo::orientacao::vertical);
        gui->defCorFundo    (        cor(0.f, 0.f, 0.f, 0.f));
    gui->fimEstilo();
    gui->adicionar<caixa>("raiz", "#topo");
        gui->defFlags       (flag_estilo::altura_justa | flag_estilo::modular);
        gui->defLargura     (                           1.0);
        gui->defOrientacao  (   estilo::orientacao::horizontal);
        gui->defCorFundo    (    cor(0.15f, 0.15f, 0.15f, 0.f));
    gui->fimEstilo();
    gui->adicionar<caixa>("raiz", "#centro");
        gui->defFlags       (flag_estilo::modular);
        gui->defLargura     (                           1.0);
        gui->defOrientacao  (   estilo::orientacao::horizontal);
        gui->defCorFundo    (    cor(0.f, 0.f, 1.f, 0.f));
        gui->defCrescimentoM(                            1.0f);
    gui->fimEstilo();
    gui->adicionar<caixa>("raiz", "#console");
        gui->defFlags       (flag_estilo::modular);
        gui->defAtivo       (false);
        gui->defLargura     (                           1.0);
        gui->defOrientacao  (   estilo::orientacao::horizontal);
        gui->defCorFundo    (    cor(0.05f, 0.05f, 0.05f, 1.f));
        gui->defCrescimentoM(                            0.2f);
    gui->fimEstilo();
    /// -- topo
    gui->adicionar<caixa>("#topo", "##barra_menu");
        gui->defFlags       (flag_estilo::altura_justa | flag_estilo::modular | flag_estilo::alinhamento_central);
        gui->defCrescimentoM(                           1.0);
        gui->defOrientacao  (   estilo::orientacao::horizontal);
        gui->defCorFundo    (    cor(0.15f, 0.15f, 0.15f, 1.f));
    gui->fimEstilo();
    gui->adicionar<caixa>("#topo", "##barra_versao");
        gui->defFlags       (flag_estilo::modular | flag_estilo::alinhamento_fim);
        gui->defCrescimentoM(                           0.2);
        gui->defAltura      (1.0);
        gui->defOrientacao  (   estilo::orientacao::horizontal);
        gui->defPaddingG    (                      5, 0);
        gui->defCorFundo    (    cor(0.11f, 0.11f, 0.11f, 1.f));
    gui->fimEstilo();
    /// -- centro
    gui->adicionar<caixa>("#centro", "##entidades");
        gui->defFlags       (           flag_estilo::modular);
        gui->defLargura     (                             35);
        gui->defAltura      (                            1.0);
        gui->defOrientacao  (   estilo::orientacao::vertical);
        gui->defPaddingG    (                           5, 5);
        gui->defCorFundo    (     cor(0.1f, 0.1f, 0.1f, 1.f));
    gui->fimEstilo();
    gui->adicionar("#centro", "##editor", std::move(cam.framebuffer_ptr));
        gui->defFlags       (flag_estilo::altura_percentual | flag_estilo::modular);
        gui->defAltura      (                            1.0);
        gui->defCorFundo    (     cor(0.0f, 0.0f, 0.0f, 0.f));
        gui->defCorBorda    (  cor(0.07f, 0.07f, 0.07f, 1.f));
        gui->defCrescimentoM(                            1.f);
        gui->defTamanhoBorda(                              4);
    gui->fimEstilo();
    gui->adicionar<caixa>("#centro", "##inspetor");
        gui->defFlags        (flag_estilo::altura_percentual | flag_estilo::modular);
        gui->defAltura       (                           1.0);
        gui->defCrescimentoM (                           0.5);
        gui->defOrientacao   (  estilo::orientacao::vertical);
        gui->defCorFundo     (    cor(0.1f, 0.1f, 0.1f, 1.f));
    gui->fimEstilo();
    /// -- console
    gui->adicionar<elementos::texto>("#console", "##output", depuracao::obterMensagens(), 14.f);
        gui->defAltura(1.0);
    gui->fimEstilo();
    /// -- -- barra menu
    gui->adicionar<elementos::botao>("##barra_menu", "###btn1", &gui->obterElemento("#console")->m_estilo.m_ativo, "Mostrar Console");
    gui->adicionar<elementos::botao>("##barra_menu", "###btn2", 
            []() {
            }
            , "Arquivo");
    gui->adicionar<elementos::botao>("##barra_menu", "###btn3", 
            []() {
            }
            , "Editar");
    gui->adicionar<elementos::botao>("##barra_menu", "###btn4", 
            []() {
            }
            , "Exibir");
    gui->adicionar<elementos::botao>("##barra_menu", "###btn5", 
            []() {
            }
            , "Primitivas", "cubo_branco");
    gui->adicionar<elementos::botao>("##barra_menu", "###btn6", 
            []() {
                abrirLink("https://d4nielstone.github.io/bubble_engine/pt/md_docs_2ajuda_2ajuda.html");
            }
            , "Documentacao");
        gui->defFlags        (flag_estilo::altura_percentual);
        gui->defCorFundo     (cor(0.11f, 0.11f, 0.11f, 1.f));
        gui->defCorBorda     (cor(0.17f, 0.17f, 0.17f, 0.f));
        gui->defAltura       (18);
        gui->defLargura      (18);
        gui->defPaddingG     (5, 0);
    gui->fimEstilo();
    /// -- -- barra versao
    gui->adicionar<elementos::texto>("##barra_versao", "###texto", std::string("| v") + std::string(BUBBLE_VERSAO_COMPLETA_STR));
        gui->defPadding    (2, 1.5);
        gui->defAltura(16);
    gui->fimEstilo();
    /// -- -- editor
    gui->adicionar<elementos::botao>("##editor", "###btn7", 
            []() {
                projeto_atual->salvarFases();
                sistema_editor::executarRuntime();
            }
            , std::make_unique<elementos::imagem>("Play.png"));
        gui->defPadding      (                        15, 15);
        gui->defLargura      (                            30);
        gui->defAltura       (                            30);
        gui->defCorFundo     (   cor(0.0f, 0.0f, 0.0f, 0.0f));
    gui->fimEstilo();
    /// -- -- inspetor
    gui->adicionar<elementos::texto>("##inspetor", "###entidade_atual", &texto_entidade, 14.f, elementos::flags_texto::alinhamento_central);
        gui->defFlags        (flag_estilo::largura_percentual);
        gui->defCorFundo     ( cor(0.07f, 0.07f, 0.07f, 1.f));
        gui->defLargura      (                           1.0);
        gui->defAltura       (                            17);
    gui->fimEstilo();
    gui->adicionar<caixa>("##inspetor", "###componentes");
        gui->defCorFundo     (  cor(0.08f, 0.08f, 0.08f, 1.f));
        gui->defFlags        (flag_estilo::largura_percentual | flag_estilo::modular);
        gui->defLargura      (                           1.0);
        gui->defAltura       (                            35);
        gui->defPaddingG     (                          5, 5);
        gui->defOrientacao   (estilo::orientacao::horizontal);
    gui->fimEstilo();
    gui->adicionar<caixa>("##inspetor", "###componente_atual");
        gui->defCorFundo     (  cor(0.08f, 0.08f, 0.08f, 0.f));
        gui->defFlags        (flag_estilo::largura_percentual | flag_estilo::modular | flag_estilo::alinhamento_central | flag_estilo::altura_justa);
        gui->defLargura      (                           1.0);
        gui->defPaddingG     (                          5, 5);
    gui->fimEstilo();
    gui->adicionar<caixa>("##inspetor", "###propriedades");
        gui->defCorFundo     (    cor(0.08f, 0.08f, 0.08f, 1.f));
        gui->defFlags        (flag_estilo::largura_percentual | flag_estilo::modular);
        gui->defLargura       (                           1.0);
        gui->defCrescimentoM (1.0);
        gui->defPaddingG     (                           5, 5);
    gui->fimEstilo();
}

void sistema_editor::inicializar(fase* f)
{
    sistema::inicializar(f);

    projeto_atual->srender()->definirCamera(&cam);
    
    // Adiciona sistema de gui ao projeto
    projeto_atual->adicionar("bubble_gui", new bubble_gui());
    gui = static_cast<bubble_gui*>(projeto_atual->obterSistema("bubble_gui"));
    /*  Config da interface   */
    configurarInterface(*projeto_atual);
    atualizarEntidades();
    atualizarComponentes();
    /*                        */
}

void sistema_editor::atualizar()
{
    // inputs
    if(janela::obterInstancia().m_inputs.isKeyPressed("F5"))
    {
        projeto_atual->salvarFases();
        executarRuntime();
    }
    if(janela::obterInstancia().m_inputs.isKeyPressed("Ctrl") && janela::obterInstancia().m_inputs.isKeyPressed("S"))
    {
        projeto_atual->salvarFases();
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
    if(janela::obterInstancia().m_inputs.isKeyPressed("Shif"))
    {
        if(gatilho_ && janela::obterInstancia().m_inputs.isKeyPressed("A"))
        {
        projeto_atual->obterFaseAtual()->obterRegistro()->criar();
        gatilho_ = false;
        }else if(gatilho_ && janela::obterInstancia().m_inputs.isKeyPressed("X"))
        {
projeto_atual->obterFaseAtual()->obterRegistro()->remover(entidade_atual);
        gatilho_ = false;
        }
        else if(!janela::obterInstancia().m_inputs.isKeyPressed("X") && !janela::obterInstancia().m_inputs.isKeyPressed("A"))
        {
        gatilho_ = true;
        }
    }
    cam.atualizarMovimentacao();
}

void sistema_editor::executarRuntime()
{
    // Inicia o runtime
    iniciarRuntime({projeto_atual->diretorioDoProjeto}); 

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
    // head
    if(!gui) {
        depuracao::emitir(erro, "interface", "sistema de gui inválido");
        return;
    }

    // Remove os botões antigos para evitar duplicatas
    gui->removerFilhos("##entidades");
    
    if(!projeto_atual->obterFaseAtual()->obterRegistro()->entidades.empty())
    {
    // Adiciona os botões para cada entidade existente
    for (auto& [id, comp] : projeto_atual->obterFaseAtual()->obterRegistro()->entidades)
    {
        std::string icone = "cube.png";
        gui->adicionar<elementos::botao>("##entidades", std::to_string(id),
            [id, this]() {
                entidade_atual = id;
                texto_entidade = "id:" + std::to_string(entidade_atual);
            }
            , std::make_unique<elementos::imagem>(icone));
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
    gui->fimEstilo();
    atualizarComponentes();
    atualizarComponente(componente::COMPONENTE_TRANSFORMACAO);
}
void sistema_editor::atualizarComponentes()
{
    if (!gui) return;

    // Remove os botões antigos para evitar duplicatas
    gui->removerFilhos("###componentes");
    if(entidade_atual == 0) return;
    size_t i = 0;
    for (auto& [mascara, componente] : projeto_atual->obterFaseAtual()->obterRegistro()->entidades[entidade_atual]) 
    {
        i++;

        std::string nome = "cube";
        
        if(componente::mapa_nomes_componentes.find(mascara) != componente::mapa_nomes_componentes.end())
            nome = componente::mapa_nomes_componentes[mascara];
        gui->adicionar<elementos::botao>("###componentes", nome,
            [mascara, this]() {
                atualizarComponente(mascara);
            }
            , std::make_unique<elementos::imagem>(nome + ".png")
        );
    }
    gui->defLargura     (                          25);
    gui->defAltura      (                          25);
    gui->defCorFundo    (    cor(0.0f, 0.0f, 0.0f, 0.f));
    gui->fimEstilo();
}

void sistema_editor::atualizarComponente(const componente::mascara& mascara)
{
    if (!gui) return;

    // Remove os botões antigos para evitar duplicatas
    gui->removerFilhos("###componente_atual");
    gui->removerFilhos("###propriedades");
    std::string nome = "nenhum";
    if(componente::mapa_nomes_componentes.find(mascara) != componente::mapa_nomes_componentes.end())
        nome = componente::mapa_nomes_componentes[mascara];
        
    gui->adicionar<elementos::texto>("###componente_atual", "####texto", nome);
    gui->fimEstilo();

    if(entidade_atual == 0) return;
    
    auto registro = projeto_atual->obterFaseAtual()->obterRegistro();

    switch (mascara) {
        case componente::COMPONENTE_CAM:
            gui->adicionar<elementos::botao>(
                    "###propriedades",
                    "####btn_cam",
                    &registro->obter<camera>(entidade_atual)->m_use_skybox, "Ativar/Destativar Skybox", "Camera.png");
                gui->defCorFundo    (cor(0.11f, 0.11f, 0.11f, 1.f));
                gui->defCorBorda    (cor(0.27f, 0.27f, 0.27f, 0.f));
                gui->defCrescimentoM(                          1.0);
            gui->fimEstilo();
            break;
        case componente::COMPONENTE_CODIGO:
            gui->adicionar<elementos::botao>(
                    "###propriedades",
                    "####btn_editor",
                    [registro, this](){ abrirNoTerminal(obterEDT(), registro->obter<codigo>(entidade_atual)->arquivoCompleto); }, "Editar Script (" + obterEDT() + ")", "Codigo.png");
                gui->defCorFundo    (cor(0.11f, 0.11f, 0.11f, 1.f));
                gui->defCorBorda    (cor(0.27f, 0.27f, 0.27f, 0.f));
                gui->defCrescimentoM(                          1.0);
            gui->fimEstilo();
            break;
        default:
            break;
    }
}
