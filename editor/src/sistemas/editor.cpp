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
#include "becommons/becommons.hpp"
#include "sistemas/editor.hpp"
#include "util/runtime.hpp"

using namespace becommons;
using namespace EDITOR_NS;
bool gatilho_ = true;

inline void abrirLink(const std::string& url) {
    depuracao::emitir(info, "Abrindo ajuda...");
    std::string comando = "xdg-open \\" + url;
    system(comando.c_str());
}

void sistema_editor::configurarInterface(becommons::projeto& proj)
{
    // head
    if(!gui) {
        depuracao::emitir(erro, "interface", "sistema de gui inválido");
        return;
    }

    gui->adicionarFlags("raiz", flag_estilo::modular);
    gui->obterElemento("raiz")->m_orientacao_modular = caixa::orientacao::vertical;

    gui->obterElemento("raiz")->m_cor_fundo = cor(0.15f, 0.15f, 0.15f, 1.f);
    // menu
    gui->novoEstilo();
        gui->adicionarElemento<caixa>("raiz", "menu");
        gui->defFlags       (flag_estilo::modular | flag_estilo::alinhamento_central);
        gui->defAltura      (                            30);
        gui->defLargura     (                           1.0);
        gui->defOrientacao  (   caixa::orientacao::horizontal);
        gui->defPaddingG    (                      5, 5);
        gui->defCorFundo    (    cor(0.15f, 0.15f, 0.15f, 1.f));
    gui->novoEstilo();
        gui->adicionarElemento<caixa>("raiz", "raiz_b");
        gui->defFlags       (flag_estilo::modular);
        gui->defLargura     (                           1.0);
        gui->defOrientacao  (   caixa::orientacao::horizontal);
        gui->defCorFundo    (    cor(0.f, 0.f, 1.f, 0.f));
        gui->defCrescimentoM(                            1.0f);
    gui->novoEstilo();
        gui->adicionarElemento<caixa>("raiz", "raiz_c");
        gui->defFlags       (flag_estilo::modular);
        gui->defLargura     (                           1.0);
        gui->defOrientacao  (   caixa::orientacao::horizontal);
        gui->defCorFundo    (    cor(0.05f, 0.05f, 0.05f, 1.f));
        gui->defCrescimentoM(                            0.2f);
    gui->novoEstilo();
        gui->adicionarElemento<elementos::texto>("raiz_c", "console", depuracao::obterMensagens(), 10.f);
        gui->defAltura(1.0);
        // items menu
    gui->novoEstilo();
        gui->adicionarElemento<elementos::botao>("menu", "rc", &gui->obterElemento("raiz_c")->m_ativo, "Mostrar Console");
        gui->obterElemento("raiz_c")->m_ativo = false;
        gui->obterElemento("raiz_c")->m_crescimento_modular = 1.f;
        gui->adicionarElemento<elementos::botao>("menu", "arquivo", [](){}, "Arquivo");
        gui->adicionarElemento<elementos::botao>("menu", "editar", [](){}, "Editar");
        gui->adicionarElemento<elementos::botao>("menu", "visualizar", [](){}, "Exibir");
        gui->adicionarElemento<elementos::botao>("menu", "ajuda", [](){
                abrirLink("https://d4nielstone.github.io/bubble_engine/pt/md_docs_2ajuda_2ajuda.html");
                }, "Ajuda");
        gui->adicionarElemento<elementos::botao>("menu", "add_primitivas", [](){}, std::make_unique<elementos::imagem>("cubo_branco"));
        gui->defCorBorda    (cor(1.f, 1.f, 1.f, 1.f));
        gui->defCorFundo    (cor(0.15f, 0.15f, 0.15f, 1.f));
        gui->defPaddingG    (5, 5);
        gui->defAltura(22);
        gui->defLargura(22);
    // entidades
    gui->novoEstilo();
        gui->adicionarElemento<caixa>("raiz_b", "entidades");
        
        gui->defFlags       (flag_estilo::modular);
        gui->defLargura     (                          35);
        gui->defAltura      (                           1.0);
        gui->defOrientacao  (   caixa::orientacao::vertical);
        gui->defPaddingG    (                      5, 5);
        gui->defCorFundo    (    cor(0.1f, 0.1f, 0.1f, 1.f));

    // editor
    gui->novoEstilo();
        gui->adicionarElemento<elementos::imagem>("raiz_b", "imagem_editor", cam.textura, true);
        
        gui->defFlags       (flag_estilo::altura_percentual | flag_estilo::modular);
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
                    projeto_atual->salvarFases();
                    sistema_editor::executarRuntime();
                }, std::make_unique<elementos::imagem>("Play.png"));
        gui->defPadding      (15, 15);
        gui->defLargura      (        30);
        gui->defAltura       (        30);
        gui->defCorFundo     (cor(0.0f, 0.0f, 0.0f, 0.0f));
    gui->novoEstilo();
        gui->adicionarElemento<caixa>("raiz_b", "componentes");
        gui->defFlags        (flag_estilo::altura_percentual | flag_estilo::modular);
        gui->defAltura       (                           1.0);
        gui->defCrescimentoM (0.5);
        gui->defOrientacao   ( caixa::orientacao::vertical);
        gui->defCorFundo     (    cor(0.1f, 0.1f, 0.1f, 1.f));
    // texto
    gui->novoEstilo();
        gui->adicionarElemento<elementos::texto>("componentes", "texto2", &texto_entidade, 14.f, elementos::flags_texto::alinhamento_central);
        gui->defCorFundo     (cor(0.07f, 0.07f, 0.07f, 1.f));
        gui->defFlags        (flag_estilo::largura_percentual );
        gui->defLargura      (1.0);
        gui->defAltura       (17);
    gui->novoEstilo();
        gui->adicionarElemento<caixa>("componentes", "area_comps");
        gui->defCorFundo     (    cor(0.08f, 0.08f, 0.08f, 1.f));
        gui->defFlags        (flag_estilo::largura_percentual | flag_estilo::modular);
        gui->defLargura       (                           1.0);
        gui->defAltura       (                           35);
        gui->defPaddingG     (                           5, 5);
        gui->defOrientacao   ( caixa::orientacao::horizontal);
    gui->novoEstilo();
        gui->adicionarElemento<caixa>("componentes", "inspetor");
        gui->defCorFundo     (    cor(0.08f, 0.08f, 0.08f, 0.f));
        gui->defFlags        (flag_estilo::largura_percentual | flag_estilo::modular | flag_estilo::alinhamento_central);
        gui->defLargura       (                           1.0);
        gui->defCrescimentoM (1.0);
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
    gui = static_cast<becommons::bubble_gui*>(projeto_atual->obterSistema("bubble_gui"));
    /*  Config da interface   */
    configurarInterface(*projeto_atual);
    atualizarEntidades();
    atualizarComponentes();
    /*                        */
}

void sistema_editor::atualizar()
{
    // inputs
    if(instanciaJanela->m_inputs.isKeyPressed("F5"))
    {
        projeto_atual->salvarFases();
        executarRuntime();
    }
    if(instanciaJanela->m_inputs.isKeyPressed("Ctrl") && instanciaJanela->m_inputs.isKeyPressed("S"))
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
    if(instanciaJanela->m_inputs.isKeyPressed("Shif"))
    {
        if(gatilho_ && instanciaJanela->m_inputs.isKeyPressed("A"))
        {
        projeto_atual->obterFaseAtual()->obterRegistro()->criar();
        gatilho_ = false;
        }else if(gatilho_ && instanciaJanela->m_inputs.isKeyPressed("X"))
        {
projeto_atual->obterFaseAtual()->obterRegistro()->remover(entidade_atual);
        gatilho_ = false;
        }
        else if(!instanciaJanela->m_inputs.isKeyPressed("X") && !instanciaJanela->m_inputs.isKeyPressed("A"))
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
            std::make_unique<elementos::imagem>(icone));
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
    atualizarComponente(componente::COMPONENTE_TRANSFORMACAO);
}
void sistema_editor::atualizarComponentes()
{
    if (!gui) return;

    // Remove os botões antigos para evitar duplicatas
    gui->removerFilhos("area_comps");
    if(entidade_atual == 0) return;
    gui->novoEstilo();
    size_t i = 0;
    for (auto& [mascara, componente] : projeto_atual->obterFaseAtual()->obterRegistro()->entidades[entidade_atual]) 
    {
        i++;
        std::string nome = "cube";
        
        if(becommons::componente::mapa_nomes_componentes.find(mascara) != becommons::componente::mapa_nomes_componentes.end())
            nome = becommons::componente::mapa_nomes_componentes[mascara];
        gui->adicionarElemento<elementos::botao>(
            "area_comps",
            nome,
            [mascara, this](){
                atualizarComponente(mascara);
            },
            std::make_unique<elementos::imagem>(nome + ".png")
        );
    }
    gui->defLargura     (                          25);
    gui->defAltura      (                          25);
    gui->defCorFundo    (    cor(0.0f, 0.0f, 0.0f, 0.f));
}

void sistema_editor::atualizarComponente(const becommons::componente::mascara& mascara)
{
    if (!gui) return;

    // Remove os botões antigos para evitar duplicatas
    gui->removerFilhos("inspetor");
    if(entidade_atual == 0) return;
    std::string nome = "nenhum";
    if(becommons::componente::mapa_nomes_componentes.find(mascara) != becommons::componente::mapa_nomes_componentes.end())
        nome = becommons::componente::mapa_nomes_componentes[mascara];
    gui->novoEstilo();
        gui->adicionarElemento<elementos::texto>("inspetor", "texto_comp", nome);
    gui->novoEstilo();
    /*
    switch (mascara) {
        case componente::COMPONENTE_CAMERA:
            gui->adicionarElemento<
        default:
            break;
    }*/
}
