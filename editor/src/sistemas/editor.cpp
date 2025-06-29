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
#include <filesystem>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

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

sistema_editor::sistema_editor() : m_salvar_ao_fechar(true) {
}

// waraper da barra c_entidades.
caixa* c_entidades, *c_inspetor;

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
    
    auto img1 = std::make_unique<elementos::imagem>("info.png");
    img1->m_estilo.m_largura = 18;
    img1->m_estilo.m_altura = 18;
    estilo& e1 = barra_menu->adicionar<elementos::botao>([]() {
            becommons::abrirLink("https://d4nielstone.github.io/bubble_engine/pt/md_docs_2ajuda_2ajuda.html");
            }
            , std::move(img1))->m_estilo;
    e1.m_cor_borda = cor(0.3f);
    e1.m_cor_fundo = cor(0.11f);
    e1.m_padding_geral = {2, 2};

    auto img2 = std::make_unique<elementos::imagem>("cube.png");
    img2->m_estilo.m_largura = 18;
    img2->m_estilo.m_altura = 18;
    estilo& e2 = barra_menu->adicionar<elementos::botao>([]() {
            }
            , std::move(img2))->m_estilo;
    e2.m_cor_borda = cor(0.3f);
    e2.m_cor_fundo = cor(0.11f);
    e2.m_padding_geral = {2, 2};

    // centro
    auto* center = ui.m_raiz->adicionar<caixa>();
    c_entidades = center->adicionar<caixa>();
    center->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_percentual;
    center->m_estilo.m_largura = 1;
    center->m_estilo.m_altura = 1;
    auto framebuffer_ptr = std::make_unique<elementos::imagem>(cam.textura, true);
    framebuffer_ptr->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_percentual;
    framebuffer_ptr->m_estilo.m_largura = 1;
    framebuffer_ptr->m_estilo.m_altura = 1;
    framebuffer_ptr->m_estilo.m_cor_borda = cor(0.1f);
    cam.viewport_ptr = &framebuffer_ptr->m_imagem_tamanho;
    center->adicionar(std::move(framebuffer_ptr));
    c_inspetor = center->adicionar<caixa>();
    
    c_entidades->m_estilo.m_orientacao_modular = estilo::orientacao::vertical;
    c_entidades->m_estilo.m_flag_estilo |= flag_estilo::altura_percentual | flag_estilo::largura_justa;
    c_entidades->m_estilo.m_altura = 1;
    c_entidades->m_estilo.m_cor_fundo = cor(0.1f);
    c_entidades->m_estilo.m_padding_geral = {2, 2};

    c_inspetor->m_estilo.m_flag_estilo |= flag_estilo::altura_percentual;
    c_inspetor->m_estilo.m_largura = 200;
    c_inspetor->m_estilo.m_altura = 1;
    c_inspetor->m_estilo.m_cor_fundo = cor(0.1f);
    c_inspetor->m_estilo.m_cor_borda = cor(0.07f);
    c_inspetor->m_estilo.m_padding_geral = {2, 2};
    
    estilo e3;
    e3.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::alinhamento_central;
    e3.m_cor_borda = cor(0.3f);
    e3.m_largura = 1;
    e3.m_padding_geral = {5, 0};

    c_inspetor->adicionar<elementos::botao>([this]() {
        }, std::make_unique<elementos::imagem>("remover.png", false, 0.4))->m_estilo = e3;
    c_inspetor->adicionar<elementos::botao>([this]() {
        }, std::make_unique<elementos::imagem>("adicionar.png", false, 0.4))->m_estilo = e3;
}

void sistema_editor::inicializar() {
    sistema::inicializar();
    // \brief analizar camera do editor
    auto _usr = projeto_atual->diretorioDoProjeto + "/usr";
	std::stringstream sb;
	if (std::filesystem::exists(_usr + "/cam.json")) {
		std::ifstream file(_usr + "/cam.json");
        sb << file.rdbuf();

        rapidjson::Document doc;
	    doc.Parse(sb.str().c_str());
    	if (doc.HasParseError())  {
		    depuracao::emitir(erro, "parse da camera do editor");
	    } else {
	        if(!cam.analizar(doc))
		        depuracao::emitir(erro, "analize da camera do editor");
	    }
	}
    projeto_atual->m_render.definirCamera(&cam);
    ui.inicializar();
    adicionarCaixas();
}

void sistema_editor::salvarEditor() {
    auto _usr = projeto_atual->diretorioDoProjeto + "/usr";
    if(!std::filesystem::exists(_usr))
        std::filesystem::create_directory(_usr);

    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    cam.serializar(doc, allocator);

    // escrevendo no arquivo
    std::ofstream ofs(_usr + "/cam.json");
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    ofs << buffer.GetString();
}
        
void sistema_editor::chamarInputs() {
    // \brief salva e roda
    if(inputs::obter(inputs::F1)) {
        salvarEditor();
        projeto_atual->salvarFases();
        executarRuntime();
    }
    // \brief apenas roda a versão mais recente
    if(inputs::obter(inputs::F2)) {
        executarRuntime();
    }
    if(inputs::obter(inputs::E_CTRL)) {
        if(gatilho_ && inputs::obter(inputs::A)) {
        projeto_atual->obterFaseAtual()->obterRegistro()->criar();
        gatilho_ = false;
        }else if(gatilho_ && inputs::obter(inputs::X)) {
projeto_atual->obterFaseAtual()->obterRegistro()->remover(entidade_atual);
        gatilho_ = false;
        }else if(gatilho_ && inputs::obter(inputs::S)) {
        salvarEditor();
        projeto_atual->salvarFases();
        gatilho_ = false;
        }
        else if(!inputs::obter(inputs::X) && !inputs::obter(inputs::A) && !inputs::obter(inputs::S)) {
        gatilho_ = true;
        }
    }
    cam.atualizarMovimentacao();
    sistema_renderizacao::calcularTransformacao(cam.transform);
    if(m_salvar_ao_fechar && janela::deveFechar()) {
        salvarEditor();
    }
}

void sistema_editor::atualizar() {
    chamarInputs();
 
    // atualiza caixas
    // \{
    // atualiza c_entidades
    size_t num_entidades_atual = projeto_atual->obterFaseAtual()->obterRegistro()->entidades.size();
    if (num_entidades_atual != num_entidades_anterior) {
        entidade_atual = projeto_atual->obterFaseAtual()->obterRegistro()->entidades.empty() ? 0 : projeto_atual->obterFaseAtual()->obterRegistro()->entidades.end()->first;
        num_entidades_anterior = num_entidades_atual; // Atualiza a referência
        c_entidades->m_filhos.clear();
        for (auto& ent : projeto_atual->obterFaseAtual()->obterRegistro()->entidades) {
            c_entidades->adicionar<elementos::botao>([this, ent]() {
                entidade_atual = ent.first;
            }, std::make_unique<elementos::imagem>("cube.png", false, 0.2f));
        }
    }
    //  Verifica se a entidade atual mudou
    if (entidade_anterior != entidade_atual) {
        entidade_anterior = entidade_atual;
    }
    //
    // \}
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
