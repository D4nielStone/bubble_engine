/** @copyright 
 * MIT License
 * Copyright (c) 2025 Daniel Oliveira
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 * @file editor.cpp
 */

#include <cstdlib>
#include "util/versao.hpp"
#include "becommons/becommons.hpp"
#include "sistemas/editor.hpp"
#include "util/runtime.hpp"

using namespace becommons;
using namespace EDITOR_NS;

bool gatilho_ = true;

// raiz
// -- topo
// -- -- barra menu
// -- -- barra versao
// -- centro
// -- -- entidades
// -- -- editor
// -- -- componentes
// -- console
// -- -- output


sistema_editor::sistema_editor() {
}

void sistema_editor::adicionarCaixas() {
    // raiz
    ui.m_raiz->m_estilo.m_orientacao_modular = estilo::orientacao::vertical;
    auto* topo = ui.m_raiz->adicionar<caixa>();
    topo->m_estilo.m_flag_estilo |= flag_estilo::altura_justa | flag_estilo::largura_percentual;
    topo->m_estilo.m_largura = 1;
    // -- topo
    auto* barra_menu = topo->adicionar<caixa>();
    auto* barra_versao = topo->adicionar<caixa>();

    // barra versao
    barra_versao->m_estilo.m_flag_estilo |= flag_estilo::altura_justa | flag_estilo::largura_justa;
    barra_versao->adicionar<elementos::texto>(BUBBLE_VERSAO_COMPLETA_STR);
    barra_versao->m_estilo.m_cor_fundo = cor(0.11f);
    barra_versao->m_estilo.m_padding_geral = {5, 5};

    // barra menu
    barra_menu->m_estilo.m_flag_estilo |= flag_estilo::altura_justa | flag_estilo::alinhamento_central | flag_estilo::largura_percentual;
    barra_menu->m_estilo.m_largura = 1;
    barra_menu->m_estilo.m_padding_geral = {5, 5};
    barra_menu->m_estilo.m_cor_fundo = cor(0.15f);

    auto img = std::make_unique<elementos::imagem>("folder.png");
    img->m_estilo.m_largura = 18;
    img->m_estilo.m_altura = 18;
    estilo& e = barra_menu->adicionar<elementos::botao>([]() {
            }
            , std::move(img))->m_estilo;
    e.m_cor_borda = cor(0.3f);
    e.m_cor_fundo = cor(0.11f);
    e.m_padding_geral = {2, 2};

    // centro
    auto* center = ui.m_raiz->adicionar<caixa>();
    center->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_percentual;
    center->m_estilo.m_largura = 1;
    center->m_estilo.m_altura = 1;

    // veiwport do editor
    cam.framebuffer_ptr->m_estilo.m_cor_borda = cor(0.11f);
    cam.framebuffer_ptr->m_estilo.m_espessura_borda = 4;
    center->adicionar(std::move(cam.framebuffer_ptr));
}

void sistema_editor::inicializar() {
    projeto_atual->srender()->definirCamera(&cam);
    ui.inicializar();
    adicionarCaixas();
}
        
void sistema_editor::chamarInputs() {
    if(inputs::obter(inputs::F1)) {
        projeto_atual->salvarFases();
        executarRuntime();
    }
    if(inputs::obter(inputs::F2)) {
        executarRuntime();
    }
    if(inputs::obter(inputs::E_SHIFT)) {
        if(gatilho_ && inputs::obter(inputs::A)) {
        projeto_atual->obterFaseAtual()->obterRegistro()->criar();
        gatilho_ = false;
        }else if(gatilho_ && inputs::obter(inputs::X)) {
projeto_atual->obterFaseAtual()->obterRegistro()->remover(entidade_atual);
        gatilho_ = false;
        }else if(gatilho_ && inputs::obter(inputs::S)) {
        projeto_atual->salvarFases();
        gatilho_ = false;
        }
        else if(!inputs::obter(inputs::X) && !inputs::obter(inputs::A) && !inputs::obter(inputs::S)) {
        gatilho_ = true;
        }
    }
    cam.atualizarMovimentacao();
    sistema_renderizacao::calcularTransformacao(cam.transform);
}

void sistema_editor::atualizar() {
    chamarInputs();
    // Verifica se o número de entidades mudou
    size_t num_entidades_atual = projeto_atual->obterFaseAtual()->obterRegistro()->entidades.size();
    if (num_entidades_atual != num_entidades_anterior) {
        num_entidades_anterior = num_entidades_atual; // Atualiza a referência
    }
    // Verifica se a entidade atual mudou
    if (entidade_anterior != entidade_atual) {
        entidade_anterior = entidade_atual;
    }
    ui.atualizar();
}

void sistema_editor::executarRuntime() {
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

void sistema_editor::monitorarRuntime() {
    while (rodando.load()) {
        if (!runtimeRodando()) // Verifica se o runtime ainda está ativo {
            rodando.store(false);
            break;
        }
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
}
