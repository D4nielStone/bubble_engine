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
#include <filesystem>
#include <typeinfo>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include "elementos/barra_menu.hpp"
#include "elementos/gerenciador_projetos.hpp"
#include "elementos/painel.hpp"
#include "becommons/becommons.hpp"

using namespace becommons;
using namespace beeditor;

/**
 * @brief Construtor da classe sistema_editor.
 * Inicializa a flag m_salvar_ao_fechar para garantir que o editor salve
 * as configurações ao ser encerrado.
 */
sistema_editor::sistema_editor() : m_salvar_ao_fechar(true) {
}
        
/**
 * @brief Adiciona e configura as caixas (containers) da interface do usuário do editor.
 * Esta função estrutura a UI em seções lógicas como topo, centro e console,
 * e configura seus estilos e comportamentos iniciais.
 */
void sistema_editor::carregarConfiguracaoPadrao()
{
    ui->resetRoot();
    // Configuração da caixa raiz (root UI element)
    ui->obterRaiz()->m_estilo.m_orientacao_modular = estilo::orientacao::vertical;

    // Configuração da seção superior da UI (barra de menu e versão)
    auto* menu = ui->obterRaiz()->adicionar<custom::barra_menu>();
    
    auto* file_sec = menu->adicionar_botao(" file ", [](){});
    menu->adicionar_botao(" gerenciador de projetos", [](){
        carregarConfiguracaoGerenciador();
            });

    // Configurando ambiente com containers
    // O container principal [dock] irá preencher a tela a-baixo da menubar
    dock = ui->obterRaiz()->adicionar<container>();
    dock->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_percentual;
    dock->m_estilo.m_largura = 1;
    dock->m_estilo.m_altura = 1;
    
    auto [top, bottom] = dock->split(0.6, estilo::orientacao::vertical);
    auto [esquerda_panel, meio_dock] = top->split(0.2);
    auto [meio_panel, direita_panel] = meio_dock->split(0.8);
    m_entidades = esquerda_panel->nova_tab<paineis::entity>();
    m_editor = meio_panel->nova_tab<paineis::editor>(cam.get());
    m_inspetor = direita_panel->nova_tab<paineis::inspector>();
    m_files = bottom->nova_tab<paineis::file_manager>();
    bottom->nova_tab<paineis::coding>();

    // Inserção do popup no loop
    auto* popup_file = ui->adicionar<elementos::popup>(file_sec);
    popup_file->adicionar<elementos::botao>([](){
            motor::obter().finalizar();
            }, "exit");
    popup_file->adicionar<elementos::botao>([](){
            if(motor::obter().m_levelmanager)
            motor::obter().m_levelmanager->salvarTudo();
            motor::obter().m_editor->salvarEditor();
            }, "save all");
}
        
void sistema_editor::carregarConfiguracaoGerenciador(){
    // Configuração da caixa raiz (root UI element)
    ui->obterRaiz()->m_estilo.m_orientacao_modular = estilo::orientacao::vertical;

    // Configuração da seção superior da UI (barra de menu e versão)
    auto* menu = ui->obterRaiz()->adicionar<custom::barra_menu>();
    
    auto* file_sec = menu->adicionar_botao(" file ", [](){});
    auto* cont = ui->obterRaiz()->adicionar<container>();
    cont->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_percentual;
    cont->m_estilo.m_largura = 1;
    cont->m_estilo.m_altura = 1;
    cont->gerarHeader();
    cont->nova_tab<EDITOR_NS::gerenciador_projetos>(obterDiretorioHome() + "/bubble");
}
/**
 * @brief Salva as configurações da câmera e do estado do editor em um arquivo JSON.
 * Cria o diretório 'usr' se não existir e serializa os dados da câmera.
 */
void sistema_editor::salvarEditor() {
    if(!motor::obter().m_projeto) return;
    auto _usr = motor::obter().m_projeto->m_diretorio + "/.usr";
    if(!std::filesystem::exists(_usr))
        std::filesystem::create_directory(_usr); // Cria o diretório se não existir

    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    cam->serializar(doc, allocator); // Serializa os dados da câmera

    // Escreve os dados serializados em um arquivo JSON.
    std::ofstream ofs(_usr + "/cam.json");
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    ofs << buffer.GetString();
}
        

/**
 * @brief Atualiza o estado do editor em cada frame.
 * Gerencia inputs, atualiza as caixas da UI, sincroniza a lista de entidades
 * e posiciona os ícones de entidades no viewport.
 */

void sistema_editor::atualizar() {
    cam->atualizarMovimentacao(); // Atualiza a posição e orientação da câmera
    sistema_renderizacao::calcularTransformacao(cam->transform); // Recalcula a matriz de transformação da câmera
    if(m_salvar_ao_fechar && motor::obter().m_janela->deveFechar()) salvarEditor();

    ui->atualizar();
    ui->renderizar();
}
/**
 * @brief Inicializa o sistema do editor.
 * Carrega a configuração da câmera do editor, define a câmera de renderização
 * e inicializa a interface do usuário, adicionando suas caixas.
 */
void sistema_editor::inicializar() {
    sistema::inicializar(); // Chama a inicialização da classe base
    ui = std::make_shared<interface>(motor::obter().m_janela.get());
    cam = std::make_shared<camera_editor>();

    ui->inicializar();

    if (motor::obter().m_projeto) {
		abrirProjeto(motor::obter().m_projeto.get());
	}
}

void sistema_editor::abrirProjeto(becommons::projeto* proj) {
		
    if (!proj)  {
		throw std::runtime_error("motor: Projeto inválido carregado.");
	}
    // Carrega a configuração da câmera do editor a partir de um arquivo JSON.
    // Se o arquivo não existir ou houver erro de parse, um erro é emitido.
    auto _usr = proj->m_diretorio + "/.usr";
    std::stringstream sb;
    if (std::filesystem::exists(_usr + "/cam.json")) {
        std::ifstream file(_usr + "/cam.json");
        sb << file.rdbuf();

        rapidjson::Document doc;
        doc.Parse(sb.str().c_str());
        if (doc.HasParseError())  {
            depuracao::emitir(erro, "parse da camera do editor");
        } else {
            if(!cam->analizar(doc))
                depuracao::emitir(erro, "analize da camera do editor");
        }
    }

    //// Carrega fase de forma asíncrona
    motor::obter().m_levelmanager->carregarAsync(motor::obter().m_projeto->m_lancamento);
    motor::obter().m_renderer->definirCamera(cam.get()); // Define a câmera do editor para renderização
}

/**
 * @brief Inicia a execução do runtime do projeto em uma thread separada.
 * Impede múltiplas instâncias do runtime rodando simultaneamente.
 *//*
void sistema_editor::executarRuntime() {
    // Inicia o runtime com o diretório do projeto
    iniciarRuntime({motor::obter().m_projeto->m_diretorio});

    // Se já houver uma thread rodando, não cria outra
    if (rodando.load()) return;

    // Marca o runtime como ativo
    rodando.store(true);

    // Inicia a thread de monitoramento do runtime em segundo plano
    threadRuntime = std::thread(&sistema_editor::monitorarRuntime);
    threadRuntime.detach(); // Libera a thread para rodar independentemente
}
*/
/**
 * @brief Monitora o estado de execução do runtime em segundo plano.
 * A thread de monitoramento verifica periodicamente se o runtime ainda está ativo.
 * Quando o runtime encerra, a flag 'rodando' é definida como false.
 *//*
void sistema_editor::monitorarRuntime() {
    while (rodando.load()) {
        if (!runtimeRodando()) { // Verifica se o runtime ainda está ativo
            rodando.store(false); // Define a flag como false se o runtime parou
            break; // Sai do loop de monitoramento
        }
        std::this_thread::sleem_for(std::chrono::milliseconds(500)); // Aguarda 500ms antes da próxima verificação
    }
}*/
