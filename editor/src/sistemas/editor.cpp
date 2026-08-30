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

sistema_editor::sistema_editor() {
}

void sistema_editor::adicionarCaixas() {
    ui.m_raiz->m_estilo.m_orientacao_modular = estilo::orientacao::vertical;

    auto* topo = ui.m_raiz->adicionar<caixa>();

    topo->m_estilo.m_flag_estilo |=
        flag_estilo::altura_justa |
        flag_estilo::largura_percentual;

    topo->m_estilo.m_largura = 1;

    auto* barra_menu = topo->adicionar<caixa>();
    auto* barra_versao = topo->adicionar<caixa>();

    barra_versao->m_estilo.m_flag_estilo |=
        flag_estilo::altura_justa |
        flag_estilo::largura_justa;

    barra_versao->adicionar<elementos::texto>(
        BUBBLE_VERSAO_COMPLETA_STR
    );

    barra_versao->m_estilo.m_cor_fundo = cor(0.11f);
    barra_versao->m_estilo.m_padding_geral = {5, 5};

    barra_menu->m_estilo.m_flag_estilo |=
        flag_estilo::altura_justa |
        flag_estilo::alinhamento_central |
        flag_estilo::largura_percentual;

    barra_menu->m_estilo.m_largura = 1;
    barra_menu->m_estilo.m_padding_geral = {5, 5};
    barra_menu->m_estilo.m_cor_fundo = cor(0.15f);

    auto img = std::make_unique<elementos::imagem>("folder.png");

    img->m_estilo.m_largura = 18;
    img->m_estilo.m_altura = 18;

    estilo& e = barra_menu->adicionar<elementos::botao>(
        []() {
        },
        std::move(img)
    )->m_estilo;

    e.m_cor_borda = cor(0.3f);
    e.m_cor_fundo = cor(0.11f);
    e.m_padding_geral = {2, 2};

    auto* center = ui.m_raiz->adicionar<caixa>();

    center->m_estilo.m_orientacao_modular =
        estilo::orientacao::horizontal;

    center->m_estilo.m_flag_estilo |=
        flag_estilo::largura_percentual |
        flag_estilo::altura_percentual;

    center->m_estilo.m_largura = 1;
    center->m_estilo.m_altura = 1;

    auto* painel_entidades = center->adicionar<caixa>();

    painel_entidades->m_estilo.m_flag_estilo |=
        flag_estilo::altura_percentual;

    painel_entidades->m_estilo.m_altura = 1;
    painel_entidades->m_estilo.m_largura = 0.20f;

    painel_entidades->m_estilo.m_cor_fundo = cor(0.10f);
    painel_entidades->m_estilo.m_cor_borda = cor(0.18f);
    painel_entidades->m_estilo.m_espessura_borda = 1;
    painel_entidades->m_estilo.m_padding_geral = {5, 5};

    auto* titulo_entidades =
        painel_entidades->adicionar<elementos::texto>("Entidades");

    titulo_entidades->m_estilo.m_padding_geral = {5, 5};

    cam.framebuffer_ptr->m_estilo.m_cor_borda = cor(0.11f);
    cam.framebuffer_ptr->m_estilo.m_espessura_borda = 4;

    auto* viewport =
        center->adicionar(std::move(cam.framebuffer_ptr));

    viewport->m_estilo.m_flag_estilo |=
        flag_estilo::altura_percentual;

    viewport->m_estilo.m_altura = 1;
}

void sistema_editor::inicializar() {
    projeto_atual->srender()->definirCamera(&cam);

    ui.inicializar();

    adicionarCaixas();
}

void sistema_editor::chamarInputs() {
    if (inputs::obter(inputs::F1)) {
        projeto_atual->salvarFases();
        executarRuntime();
    }

    if (inputs::obter(inputs::F2)) {
        executarRuntime();
    }

    if (inputs::obter(inputs::E_SHIFT)) {
        if (gatilho_ && inputs::obter(inputs::A)) {
            projeto_atual
                ->obterFaseAtual()
                ->obterRegistro()
                ->criar();

            gatilho_ = false;
        }
        else if (gatilho_ && inputs::obter(inputs::X)) {
            projeto_atual
                ->obterFaseAtual()
                ->obterRegistro()
                ->remover(entidade_atual);

            gatilho_ = false;
        }
        else if (gatilho_ && inputs::obter(inputs::S)) {
            projeto_atual->salvarFases();

            gatilho_ = false;
        }
        else if (
            !inputs::obter(inputs::X) &&
            !inputs::obter(inputs::A) &&
            !inputs::obter(inputs::S)
        ) {
            gatilho_ = true;
        }
    }

    cam.atualizarMovimentacao();

    sistema_renderizacao::calcularTransformacao(
        cam.transform
    );
}

void sistema_editor::atualizar() {
    chamarInputs();

    auto* registro =
        projeto_atual
            ->obterFaseAtual()
            ->obterRegistro();

    size_t num_entidades_atual =
        registro->entidades.size();

    if (num_entidades_atual != num_entidades_anterior) {
        num_entidades_anterior =
            num_entidades_atual;
    }

    if (entidade_anterior != entidade_atual) {
        entidade_anterior =
            entidade_atual;
    }

    ui.atualizar();
}

void sistema_editor::executarRuntime() {
}

void sistema_editor::monitorarRuntime() {
    while (rodando.load()) {
        if (!runtimeRodando()) {
            rodando.store(false);
            break;
        }

        std::this_thread::sleep_for(
            std::chrono::milliseconds(500)
        );
    }
}