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
#include "elementos/popup.hpp"
#include "sistemas/editor.hpp"
#include "util/runtime.hpp"
#include <filesystem>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

using namespace becommons;
using namespace EDITOR_NS;

bool gatilho_ = true; // Usado para controlar a ativação de inputs baseados em gatilho (e.g., CTRL + A)

// Define as caixas de interface do usuário principais para organização
// raiz (root)
// -- topo (top)
// -- -- barra menu (menu bar)
// -- -- barra versao (version bar)
// -- centro (center)
// -- -- entidades (entities panel)
// -- -- editor (editor view/viewport)
// -- -- componentes (components panel - inspector)
// -- console (console/output area)
// -- -- output (output messages)

// Declaração global para acesso rápido às caixas de entidades e inspetor.
// Essas caixas são gerenciadas pela UI e contêm os elementos visuais
// para manipulação de entidades e seus componentes.
caixa* c_entidades, *c_inspetor;

// Framebuffer principal que exibe a renderização da câmera do editor.
// O viewport da câmera é ajustado para as dimensões deste framebuffer.
elementos::imagem* framebuffer;

/**
 * @brief Construtor da classe sistema_editor.
 * Inicializa a flag m_salvar_ao_fechar para garantir que o editor salve
 * as configurações ao ser encerrado.
 */
sistema_editor::sistema_editor() : m_salvar_ao_fechar(true) {
}

// Mapa para armazenar ícones de entidades no viewport, associando-os às posições 3D.
// Isso permite que os ícones se movam e escalem com a câmera do editor.
std::unordered_map<fvet3*, elementos::imagem*> icones;

/**
 * @brief Adiciona e configura as caixas (containers) da interface do usuário do editor.
 * Esta função estrutura a UI em seções lógicas como topo, centro e console,
 * e configura seus estilos e comportamentos iniciais.
 */
void sistema_editor::adicionarCaixas() {
    // Configuração da caixa raiz (root UI element)
    ui.m_raiz->m_estilo.m_orientacao_modular = estilo::orientacao::vertical;

    // Configuração da seção superior da UI (barra de menu e versão)
    auto* topo = ui.m_raiz->adicionar<caixa>();
    topo->m_estilo.m_flag_estilo |= flag_estilo::altura_justa | flag_estilo::largura_percentual;
    topo->m_estilo.m_largura = 1;

    // Configuração da barra de menu
    auto* barra_menu = topo->adicionar<caixa>();
    barra_menu->m_estilo.m_flag_estilo |= flag_estilo::altura_justa | flag_estilo::alinhamento_central | flag_estilo::largura_percentual;
    barra_menu->m_estilo.m_largura = 1;
    barra_menu->m_estilo.m_padding_geral = {5, 5};
    barra_menu->m_estilo.m_cor_fundo = cor(0.15f);

    // Configuração da barra de versão
    auto* barra_versao = topo->adicionar<caixa>();
    barra_versao->m_estilo.m_flag_estilo |= flag_estilo::altura_justa | flag_estilo::largura_justa;
    barra_versao->adicionar<elementos::texto>(BUBBLE_VERSAO_COMPLETA_STR); // Exibe a string de versão completa
    barra_versao->m_estilo.m_cor_fundo = cor(0.11f);
    barra_versao->m_estilo.m_padding_geral = {5, 5};

    // Botão "Folder" (Arquivo) com popup para ações de salvar projeto/fase
    auto img = std::make_unique<elementos::imagem>("folder.png");
    img->m_estilo.m_largura = 18;
    img->m_estilo.m_altura = 18;
    auto* btn_folder = barra_menu->adicionar<elementos::botao>([]() {
            // Ação principal do botão (pode ser vazia se as ações estiverem no popup)
        }
        , std::move(img));
    btn_folder->m_estilo.m_cor_borda = cor(0.3f);
    btn_folder->m_estilo.m_cor_fundo = cor(0.11f);
    btn_folder->m_estilo.m_padding_geral = {2, 2};
    estilo e = btn_folder->m_estilo; // Reutiliza estilo para os botões do popup

    auto* popup_folder = btn_folder->adicionar<elementos::popup>();
    popup_folder->adicionar<elementos::botao>([this]() {
            projeto_atual->salvarFases(); // Salva todas as fases do projeto
        }, "salvar projeto", "folder.png")->m_estilo = e;
    popup_folder->adicionar<elementos::botao>([this]() {
            projeto_atual->salvarFase(projeto_atual->fase_atual); // Salva apenas a fase atual
        }, "salvar fase atual", "folder.png")->m_estilo = e;

    // Botão "Info" para abrir link de ajuda
    auto img1 = std::make_unique<elementos::imagem>("info.png");
    img1->m_estilo.m_largura = 18;
    img1->m_estilo.m_altura = 18;
    estilo& e1 = barra_menu->adicionar<elementos::botao>([]() {
            becommons::abrirLink("https://d4nielstone.github.io/bubble_engine/pt/md_docs_2ajuda_2ajuda.html"); // Abre o link de documentação
        }
        , std::move(img1))->m_estilo;
    e1.m_cor_borda = cor(0.3f);
    e1.m_cor_fundo = cor(0.11f);
    e1.m_padding_geral = {2, 2};

    // Botão "Cube" (provavelmente para criar nova entidade ou abrir menu de entidades)
    auto img2 = std::make_unique<elementos::imagem>("cube.png");
    img2->m_estilo.m_largura = 18;
    img2->m_estilo.m_altura = 18;
    estilo& e2 = barra_menu->adicionar<elementos::botao>([]() {
            // Ação do botão (placeholder)
        }
        , std::move(img2))->m_estilo;
    e2.m_cor_borda = cor(0.3f);
    e2.m_cor_fundo = cor(0.11f);
    e2.m_padding_geral = {2, 2};

    // Configuração da seção central da UI (entidades, viewport e inspetor)
    auto* center = ui.m_raiz->adicionar<caixa>();
    center->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_percentual;
    center->m_estilo.m_largura = 1;
    center->m_estilo.m_altura = 1;

    // Painel de entidades (lado esquerdo)
    c_entidades = center->adicionar<caixa>();
    c_entidades->m_estilo.m_orientacao_modular = estilo::orientacao::vertical;
    c_entidades->m_estilo.m_flag_estilo |= flag_estilo::altura_percentual | flag_estilo::largura_justa;
    c_entidades->m_estilo.m_altura = 1;
    c_entidades->m_estilo.m_cor_fundo = cor(0.1f);
    c_entidades->m_estilo.m_padding_geral = {2, 2};

    // Framebuffer para a visualização da câmera do editor
    framebuffer = center->adicionar<elementos::imagem>(cam.textura, true);
    framebuffer->m_estilo.m_flag_estilo = flag_estilo::largura_percentual | flag_estilo::altura_percentual;
    framebuffer->m_estilo.m_largura = 1;
    framebuffer->m_estilo.m_altura = 1;
    framebuffer->m_estilo.m_cor_borda = cor(0.1f);
    cam.viewport_ptr = &framebuffer->m_estilo.m_limites; // Associa o viewport da câmera aos limites do framebuffer

    // Painel do inspetor (lado direito, para propriedades de entidades/componentes)
    c_inspetor = center->adicionar<caixa>();
    c_inspetor->m_estilo.m_flag_estilo |= flag_estilo::altura_percentual;
    c_inspetor->m_estilo.m_largura = 200;
    c_inspetor->m_estilo.m_altura = 1;
    c_inspetor->m_estilo.m_cor_fundo = cor(0.1f);
    c_inspetor->m_estilo.m_cor_borda = cor(0.07f);
    c_inspetor->m_estilo.m_padding_geral = {2, 2};

    // Estilo para botões de remoção e adição de componentes
    estilo e3;
    e3.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::alinhamento_central;
    e3.m_cor_borda = cor(0.3f);
    e3.m_largura = 1;
    e3.m_padding_geral = {5, 0};

    // Botão para remover componentes (placeholder)
    c_inspetor->adicionar<elementos::botao>([this]() {
        // Ação de remover componente (placeholder)
        }, std::make_unique<elementos::imagem>("remover.png", false, 0.4))->m_estilo = e3;

    // Botão para adicionar componentes com popup de seleção
    auto* btn_add_comp = c_inspetor->adicionar<elementos::botao>([this]() {
        // Ação de adicionar componente (placeholder)
        }, std::make_unique<elementos::imagem>("adicionar.png", false, 0.4));
    btn_add_comp->m_estilo = e3;
    auto* popup_comp = btn_add_comp->adicionar<elementos::popup>();

    // Opções de componentes no popup "Adicionar Componente"
    popup_comp->adicionar<elementos::botao>([this]() {
            projeto_atual->obterFaseAtual()->obterRegistro()->adicionar<codigo>(entidade_atual);
        }, "codigo", "Codigo.png")->m_estilo = e;
    popup_comp->adicionar<elementos::botao>([this]() {
            projeto_atual->obterFaseAtual()->obterRegistro()->adicionar<camera>(entidade_atual);
        }, "camera", "Camera.png")->m_estilo = e;
    popup_comp->adicionar<elementos::botao>([this]() {
            projeto_atual->obterFaseAtual()->obterRegistro()->adicionar<terreno>(entidade_atual);
        }, "terreno", "Terreno.png")->m_estilo = e;
    popup_comp->adicionar<elementos::botao>([this]() {
            projeto_atual->obterFaseAtual()->obterRegistro()->adicionar<fisica>(entidade_atual);
        }, "fisica", "Fisica.png")->m_estilo = e;
    popup_comp->adicionar<elementos::botao>([this]() {
            projeto_atual->obterFaseAtual()->obterRegistro()->adicionar<luz_direcional>(entidade_atual);
        }, "luz direcional", "Iluminacao.png")->m_estilo = e;
    popup_comp->adicionar<elementos::botao>([this]() {
            projeto_atual->obterFaseAtual()->obterRegistro()->adicionar<renderizador>(entidade_atual);
        }, "renderizador", "Renderizador.png")->m_estilo = e;
}

/**
 * @brief Inicializa o sistema do editor.
 * Carrega a configuração da câmera do editor, define a câmera de renderização
 * e inicializa a interface do usuário, adicionando suas caixas.
 */
void sistema_editor::inicializar() {
    sistema::inicializar(); // Chama a inicialização da classe base

    // Carrega a configuração da câmera do editor a partir de um arquivo JSON.
    // Se o arquivo não existir ou houver erro de parse, um erro é emitido.
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
    projeto_atual->m_render.definirCamera(&cam); // Define a câmera do editor para renderização
    ui.inicializar();
    adicionarCaixas(); // Constrói a interface do editor
}

/**
 * @brief Salva as configurações da câmera e do estado do editor em um arquivo JSON.
 * Cria o diretório 'usr' se não existir e serializa os dados da câmera.
 */
void sistema_editor::salvarEditor() {
    auto _usr = projeto_atual->diretorioDoProjeto + "/usr";
    if(!std::filesystem::exists(_usr))
        std::filesystem::create_directory(_usr); // Cria o diretório se não existir

    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    cam.serializar(doc, allocator); // Serializa os dados da câmera

    // Escreve os dados serializados em um arquivo JSON.
    std::ofstream ofs(_usr + "/cam.json");
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    ofs << buffer.GetString();
}

/**
 * @brief Gerencia os inputs do usuário para ações do editor.
 * Inclui atalhos para salvar, rodar o runtime, criar/remover entidades e salvar fases.
 * Também atualiza a movimentação da câmera do editor.
 */
void sistema_editor::chamarInputs() {
    // Salva o editor e o projeto, e inicia o runtime
    if(inputs::obter(inputs::F1)) {
        salvarEditor();
        projeto_atual->salvarFases();
        executarRuntime();
    }
    // Apenas roda a versão mais recente do runtime
    if(inputs::obter(inputs::F2)) {
        executarRuntime();
    }
    // Gerencia inputs com a tecla CTRL pressionada para evitar repetições
    if(inputs::obter(inputs::E_CTRL)) {
        if(gatilho_ && inputs::obter(inputs::A)) {
            projeto_atual->obterFaseAtual()->obterRegistro()->criar(); // Cria nova entidade
            gatilho_ = false;
        } else if(gatilho_ && inputs::obter(inputs::X)) {
            projeto_atual->obterFaseAtual()->obterRegistro()->remover(entidade_atual); // Remove a entidade selecionada
            gatilho_ = false;
        } else if(gatilho_ && inputs::obter(inputs::S)) {
            salvarEditor(); // Salva configurações do editor
            projeto_atual->salvarFases(); // Salva todas as fases do projeto
            gatilho_ = false;
        } else if(!inputs::obter(inputs::X) && !inputs::obter(inputs::A) && !inputs::obter(inputs::S)) {
            gatilho_ = true; // Reinicia o gatilho quando nenhuma das teclas é pressionada
        }
    }
    cam.atualizarMovimentacao(); // Atualiza a posição e orientação da câmera
    sistema_renderizacao::calcularTransformacao(cam.transform); // Recalcula a matriz de transformação da câmera
    if(m_salvar_ao_fechar && janela::deveFechar()) {
        salvarEditor(); // Salva o editor se a janela estiver fechando e a flag estiver ativa
    }
}

/**
 * @brief Atualiza o estado do editor em cada frame.
 * Gerencia inputs, atualiza as caixas da UI, sincroniza a lista de entidades
 * e posiciona os ícones de entidades no viewport.
 */
void sistema_editor::atualizar() {
    chamarInputs();

    // Atualiza a exibição de entidades no painel e no viewport.
    // Verifica se o número de entidades mudou para reconstruir a lista e os ícones.
    size_t num_entidades_atual = projeto_atual->obterFaseAtual()->obterRegistro()->entidades.size();
    if (num_entidades_atual != num_entidades_anterior) {
        auto reg = projeto_atual->obterFaseAtual()->obterRegistro();
        // Define a entidade atual; se não houver entidades, define como 0.
        entidade_atual = reg->entidades.empty() ? 0 : projeto_atual->obterFaseAtual()->obterRegistro()->entidades.empty() ? 0 : reg->entidades.rbegin()->first;
        num_entidades_anterior = num_entidades_atual; // Atualiza a referência para a próxima verificação

        c_entidades->m_filhos.clear(); // Limpa os botões de entidades existentes
        framebuffer->m_filhos.clear(); // Limpa os ícones de entidades existentes
        icones.clear(); // Limpa o mapa de ícones

        // Recria os botões e ícones para cada entidade
        for (auto& [id, comps] : reg->entidades) {
            c_entidades->adicionar<elementos::botao>([this, id]() {
                entidade_atual = id; // Define a entidade selecionada ao clicar no botão
            }, std::make_unique<elementos::imagem>("cube.png", false, 0.2f));

            // Adiciona o ícone da entidade no framebuffer do editor
            auto nome_comp = componente::mapa_nomes_componentes[comps.begin()->first];
            elementos::imagem* img = framebuffer->adicionar<elementos::imagem>(nome_comp+".png", false, 2);
            icones[&reg->obter<transformacao>(id)->posicao] = img; // Associa o ícone à posição da transformação da entidade
        }
    }
    // Verifica se a entidade atual mudou (para futuras atualizações do inspetor)
    if (entidade_anterior != entidade_atual) {
        entidade_anterior = entidade_atual;
        // Lógica para atualizar o inspetor com base na 'entidade_atual' pode ser adicionada aqui
    }
    // Posiciona os ícones das entidades no viewport com base nas suas posições 3D e na câmera
    for (auto& [pos, img] : icones) {
        auto vet = cam.mundoParaTela(*pos); // Converte a posição 3D para coordenadas de tela
        img->m_estilo.m_limites.x = vet.x - img->m_estilo.m_limites.z/2; // Centraliza X
        img->m_estilo.m_limites.y = vet.y - img->m_estilo.m_limites.w/2; // Centraliza Y
        // Ativa/desativa o ícone se ele estiver fora da tela (atrás da câmera)
        if(vet.z < 0)
            img->m_estilo.m_ativo = false;
        else
            img->m_estilo.m_ativo = true;
    }
    ui.atualizar(); // Atualiza a interface do usuário
}

/**
 * @brief Inicia a execução do runtime do projeto em uma thread separada.
 * Impede múltiplas instâncias do runtime rodando simultaneamente.
 */
void sistema_editor::executarRuntime() {
    // Inicia o runtime com o diretório do projeto
    iniciarRuntime({projeto_atual->diretorioDoProjeto});

    // Se já houver uma thread rodando, não cria outra
    if (rodando.load()) return;

    // Marca o runtime como ativo
    rodando.store(true);

    // Inicia a thread de monitoramento do runtime em segundo plano
    threadRuntime = std::thread(&sistema_editor::monitorarRuntime);
    threadRuntime.detach(); // Libera a thread para rodar independentemente
}

/**
 * @brief Monitora o estado de execução do runtime em segundo plano.
 * A thread de monitoramento verifica periodicamente se o runtime ainda está ativo.
 * Quando o runtime encerra, a flag 'rodando' é definida como false.
 */
void sistema_editor::monitorarRuntime() {
    while (rodando.load()) {
        if (!runtimeRodando()) { // Verifica se o runtime ainda está ativo
            rodando.store(false); // Define a flag como false se o runtime parou
            break; // Sai do loop de monitoramento
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Aguarda 500ms antes da próxima verificação
    }
}
