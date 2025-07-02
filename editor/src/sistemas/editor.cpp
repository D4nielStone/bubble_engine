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

// Estilos de caixa pré-definidos
estilo e, e3;

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

// Mapa para armazenar os botões do painel de entidades, associando-os aos IDs das entidades.
std::unordered_map<unsigned int, elementos::botao*> botoes_entidades; 

// Mapa para armazenar os ícones de entidades no viewport, associando-os aos IDs das entidades.
// O valor é o "caixa*" que representa o ícone (geralmente um botão com imagem).
std::unordered_map<unsigned int, caixa*> icones_entidades; 

// Mapa para armazenar ícones de entidades no viewport, associando-os às posições 3D.
// Isso permite que os ícones se movam e escalem com a câmera do editor.
std::unordered_map<fvet3*, caixa*> icones;

// Map para criação dos popups depois
std::unordered_map<std::string, elementos::popup*> pop_ups;

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
    btn_folder->m_estilo.m_cor_borda = cor(0.1f);
    btn_folder->m_estilo.m_cor_borda.a = 1.f;
    btn_folder->m_estilo.m_cor_fundo = cor(0.11f);
    btn_folder->m_estilo.m_padding_geral = {2, 2};
    e = btn_folder->m_estilo; // Reutiliza estilo para os botões do popup

    auto* popup_folder = btn_folder->adicionar<elementos::popup>();
    popup_folder->adicionar<elementos::botao>([this]() {
            projeto_atual->salvarFases(); // Salva todas as fases do projeto
        }, "salvar projeto", "folder.png")->m_estilo = e;
    popup_folder->adicionar<elementos::botao>([this]() {
            projeto_atual->salvarFase(projeto_atual->fase_atual); // Salva apenas a fase atual
        }, "salvar fase atual", "scene.png")->m_estilo = e;
    auto* btn_luz = popup_folder->adicionar<elementos::botao>([]()
            {}, "luz global", "Iluminacao.png");
    btn_luz->m_estilo = e;        
    // Interação especial da luz direcional
    auto lg = projeto_atual->obterFaseAtual()->luz_global;
    auto* popup = btn_luz->adicionar<elementos::popup>();
    popup->m_estilo.m_orientacao_modular = estilo::orientacao::horizontal;
    popup->adicionar<elementos::caixa_de_texto>("x", &lg->direcao.x);
    popup->adicionar<elementos::caixa_de_texto>("y", &lg->direcao.y);
    popup->adicionar<elementos::caixa_de_texto>("z", &lg->direcao.z);

    // Botão "Info" para abrir link de ajuda
    auto img1 = std::make_unique<elementos::imagem>("info.png");
    img1->m_estilo.m_largura = 18;
    img1->m_estilo.m_altura = 18;
    estilo& e1 = barra_menu->adicionar<elementos::botao>([]() {
            becommons::abrirLink("https://d4nielstone.github.io/bubble_engine/pt/md_docs_2ajuda_2ajuda.html"); // Abre o link de documentação
        }
        , std::move(img1))->m_estilo;
    e1.m_cor_borda = cor(0.1f);
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
    e2.m_cor_borda = cor(0.1f);
    e2.m_cor_fundo = cor(0.11f);
    e2.m_padding_geral = {2, 2};

    // Configuração da seção central da UI (entidades, viewport e inspetor)
    auto* center = ui.m_raiz->adicionar<caixa>();
    center->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_percentual;
    center->m_estilo.m_largura = 1;
    center->m_estilo.m_altura = 1;

    // Painel de entidades (lado esquerdo)
    c_entidades = center->adicionar<caixa>();
    c_entidades->m_estilo.m_flag_estilo |= flag_estilo::largura_justa | flag_estilo::altura_percentual;
    c_entidades->m_estilo.m_orientacao_modular = estilo::orientacao::vertical;
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
    c_inspetor->m_estilo.m_largura = 150;
    c_inspetor->m_estilo.m_altura = 1;
    c_inspetor->m_estilo.m_cor_fundo = cor(0.1f);
    c_inspetor->m_estilo.m_cor_borda = cor(0.07f);
    c_inspetor->m_estilo.m_padding_geral = {2, 2};

    // Estilo para botões de remoção e adição de componentes
    e3.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::alinhamento_central;
    e3.m_cor_borda = cor(0.1f);
    e3.m_largura = 1;
    e3.m_padding_geral = {5, 0};

    // Botão para remover componentes (placeholder)
    c_inspetor->adicionar<elementos::botao>([this]() {
        // Ação de remover componente (placeholder)
        }, std::make_unique<elementos::imagem>("remover.png", false, 0.2))->m_estilo = e3;

    // Botão para adicionar componentes com popup de seleção
    auto* btn_add_comp = c_inspetor->adicionar<elementos::botao>([this]() {
        // Ação de adicionar componente (placeholder)
        }, std::make_unique<elementos::imagem>("adicionar.png", false, 0.2));
    btn_add_comp->m_estilo = e3;
    btn_add_comp->m_estilo.m_flag_estilo |= flag_estilo::quebrar_linha;
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
 * @brief Atualiza os icones de interface interativos
 * com o editor.
 */
void sistema_editor::atualizarGizmo() {
    auto reg = projeto_atual->obterFaseAtual()->obterRegistro();

    // 1. Identificar entidades que foram removidas
    std::vector<unsigned int> ids_a_remover;
    for (auto const& [id, btn_entidade] : botoes_entidades) {
        if (reg->entidades.find(id) == reg->entidades.end()) {
            ids_a_remover.push_back(id);
        }
    }

    // 2. Remover elementos de UI das entidades que não existem mais
    for (unsigned int id_removido : ids_a_remover) {
        // Remover do painel de entidades
        auto it_btn = botoes_entidades.find(id_removido);
        if (it_btn != botoes_entidades.end()) {
            c_entidades->remover(it_btn->second); // Assume que 'remover' destrói o elemento
            botoes_entidades.erase(it_btn);
        }

        // Remover do viewport (ícone)
        auto it_icone = icones_entidades.find(id_removido);
        if (it_icone != icones_entidades.end()) {
            framebuffer->remover(it_icone->second); // Assume que 'remover' destrói o elemento
            icones_entidades.erase(it_icone);
        }
    }

    // 3. Adicionar/Atualizar entidades existentes e novas
    for (auto& [id, comps] : reg->entidades) {
        // Obter o componente de transformação (crucial para o ícone no viewport)
        auto tr_ptr = reg->obter<transformacao>(id);
        if (!tr_ptr) {
            // Se a entidade não tem transformação, não deve ter ícone no viewport
            // Mas ainda pode ter botão no painel de entidades.
            // Decida o comportamento adequado aqui: remover ícone existente se houver?
            auto it_icone = icones_entidades.find(id);
            if(it_icone != icones_entidades.end()){
                framebuffer->remover(it_icone->second);
                icones_entidades.erase(it_icone);
            }
            // Continue para a próxima iteração, mas podemos adicionar o botão do painel abaixo.
            // Para simplificar, vou manter a lógica de adicionar/atualizar dentro do mesmo bloco.
        }

        // --- Lógica para o Painel de Entidades (c_entidades) ---
        if (botoes_entidades.find(id) == botoes_entidades.end()) {
            // Entidade é nova ou não tinha botão: Adicionar
            auto* btn_entidade = c_entidades->adicionar<elementos::botao>([this, id]() {
                entidade_atual = id;
            }, std::make_unique<elementos::imagem>("cube.png", false, 0.2f)); // Talvez exibir o nome da entidade aqui?
            botoes_entidades[id] = btn_entidade;
        }
        // Se o botão já existe, não precisamos fazer nada específico, a menos que 
        // seu texto/imagem precise ser atualizado dinamicamente.


        // --- Lógica para o Viewport (framebuffer - ícones) ---
        if (tr_ptr) { // Apenas entidades com transformação têm ícone no viewport
            if (icones_entidades.find(id) == icones_entidades.end()) {
                // Entidade é nova ou não tinha ícone: Adicionar
                std::string nome_comp_icone = "cube"; // Ícone padrão
                if(!comps.empty()){
                    nome_comp_icone = componente::mapa_nomes_componentes[comps.rbegin()->first];
                    nome_comp_icone = (nome_comp_icone == "Transformacao") ? "cube" : nome_comp_icone;
                }

                auto* btn_icone = framebuffer->adicionar<elementos::botao>([this, id]() {
                    entidade_atual = id;
                }, std::make_unique<elementos::imagem>(nome_comp_icone + ".png", false, 0.2f));
                btn_icone->m_estilo.m_cor_borda.a = 0;
                icones_entidades[id] = btn_icone;

                // Interação especial da câmera
                if (nome_comp_icone == "Camera") {
                    auto cam_ptr = reg->obter<camera>(id);
                    if (cam_ptr) {
                        cam_ptr->ativarFB();
                        auto* cam_pop = btn_icone->adicionar<elementos::popup>();
                        cam_pop->m_estilo.m_padding_geral = {1, 1};
                        auto* cam_framebuffer = cam_pop->adicionar<elementos::imagem>(cam_ptr->textura, true);
                        cam_framebuffer->m_estilo.m_largura = 80;
                        cam_framebuffer->m_estilo.m_altura = 50;
                        cam_ptr->viewport_ptr = &cam_framebuffer->m_estilo.m_limites;
                    }
                }
            }
        }
    }

    // Lógica para Componentes (Inspetor) - Esta parte permanece basicamente a mesma,
    // pois é reconstruída se a entidade selecionada ou seus componentes mudam.
    size_t current_entity_component_count = 0;
    if (reg->entidades.count(entidade_atual)) {
        current_entity_component_count = reg->entidades[entidade_atual].size();
    }

    if (entidade_atual != entidade_anterior || current_entity_component_count != num_componentes_anterior) {
        c_inspetor->m_filhos.clear();
        pop_ups.clear(); 

        c_inspetor->adicionar<elementos::botao>([](){ /* Ação de remover */ }, std::make_unique<elementos::imagem>("remover.png", false, 0.2))->m_estilo = e3;
        auto* btn_add_comp = c_inspetor->adicionar<elementos::botao>([](){ /* Ação de adicionar */ }, std::make_unique<elementos::imagem>("adicionar.png", false, 0.2));
        btn_add_comp->m_estilo = e3;
        btn_add_comp->m_estilo.m_flag_estilo |= flag_estilo::quebrar_linha;
        auto* popup_comp = btn_add_comp->adicionar<elementos::popup>();

        // Popula o popup de adicionar componentes (função auxiliar seria útil aqui)
        popup_comp->adicionar<elementos::botao>([this]() { projeto_atual->obterFaseAtual()->obterRegistro()->adicionar<codigo>(entidade_atual); }, "codigo", "Codigo.png")->m_estilo = e;
        popup_comp->adicionar<elementos::botao>([this]() { projeto_atual->obterFaseAtual()->obterRegistro()->adicionar<camera>(entidade_atual); }, "camera", "Camera.png")->m_estilo = e;
        popup_comp->adicionar<elementos::botao>([this]() { projeto_atual->obterFaseAtual()->obterRegistro()->adicionar<terreno>(entidade_atual); }, "terreno", "Terreno.png")->m_estilo = e;
        popup_comp->adicionar<elementos::botao>([this]() { projeto_atual->obterFaseAtual()->obterRegistro()->adicionar<fisica>(entidade_atual); }, "fisica", "Fisica.png")->m_estilo = e;
        popup_comp->adicionar<elementos::botao>([this]() { projeto_atual->obterFaseAtual()->obterRegistro()->adicionar<renderizador>(entidade_atual); }, "renderizador", "Renderizador.png")->m_estilo = e;

        if (!reg->entidades.empty() && reg->entidades.count(entidade_atual)) {
            for (auto& [mask, comp_ptr] : reg->entidades[entidade_atual]) {
                auto nome_comp = componente::mapa_nomes_componentes[mask];
                auto* comp_btn = c_inspetor->adicionar<elementos::botao>(nullptr, nome_comp, nome_comp + ".png");
                comp_btn->m_estilo.m_flag_estilo |= flag_estilo::quebrar_linha;
                auto* comp_pop = comp_btn->adicionar<elementos::popup>();
                pop_ups[nome_comp] = comp_pop;

                comp_pop->m_estilo.m_orientacao_modular = estilo::orientacao::horizontal;
                if (nome_comp == "Transformacao") {
                    auto tr_ptr = reg->obter<transformacao>(entidade_atual);
                    if (tr_ptr) {
                        comp_pop->adicionar<elementos::texto>("posicao");
                        comp_pop->adicionar<elementos::caixa_de_texto>("x", &tr_ptr->posicao.x);
                        comp_pop->adicionar<elementos::caixa_de_texto>("y", &tr_ptr->posicao.y);
                        comp_pop->adicionar<elementos::caixa_de_texto>("z", &tr_ptr->posicao.z)->m_estilo.m_flag_estilo |= flag_estilo::quebrar_linha;
                        comp_pop->adicionar<elementos::texto>("rotacao");
                        comp_pop->adicionar<elementos::caixa_de_texto>("x", &tr_ptr->rotacao.x);
                        comp_pop->adicionar<elementos::caixa_de_texto>("y", &tr_ptr->rotacao.y);
                        comp_pop->adicionar<elementos::caixa_de_texto>("z", &tr_ptr->rotacao.z)->m_estilo.m_flag_estilo |= flag_estilo::quebrar_linha;
                        comp_pop->adicionar<elementos::texto>("escala");
                        comp_pop->adicionar<elementos::caixa_de_texto>("x", &tr_ptr->escala.x);
                        comp_pop->adicionar<elementos::caixa_de_texto>("y", &tr_ptr->escala.y);
                        comp_pop->adicionar<elementos::caixa_de_texto>("z", &tr_ptr->escala.z)->m_estilo.m_flag_estilo |= flag_estilo::quebrar_linha;
                    }
                }
                else if (nome_comp == "Camera") {
                    auto cam_ptr = reg->obter<camera>(entidade_atual);
                    if (cam_ptr) {
                        comp_pop->m_estilo.m_orientacao_modular = estilo::orientacao::vertical;
                        comp_pop->adicionar<elementos::texto>("foco de visao")                          ;
                        comp_pop->adicionar<elementos::caixa_de_texto>("...", &cam_ptr->fov)            ;
                        comp_pop->adicionar<elementos::texto>("corte proximo")                          ;
                        comp_pop->adicionar<elementos::caixa_de_texto>("...", &cam_ptr->corte_curto)    ;
                        comp_pop->adicionar<elementos::texto>("corte distante")                         ;
                        comp_pop->adicionar<elementos::caixa_de_texto>("...", &cam_ptr->corte_longo)    ;
                        comp_pop->adicionar<elementos::texto>("escala ortografica")                     ;
                        comp_pop->adicionar<elementos::caixa_de_texto>("...", &cam_ptr->escala)         ;
                        comp_pop->adicionar<elementos::botao>(&cam_ptr->m_use_skybox, "usar skybox", "Camera.png");
                        comp_pop->adicionar<elementos::botao>(&cam_ptr->flag_orth, "ortografica", "Camera.png");
                    }
                }
            }
        }
        num_componentes_anterior = current_entity_component_count;
        entidade_anterior = entidade_atual;
    }

    // 4. Posicionar os ícones das entidades no viewport (sempre atualizado, mesmo se não houver adição/remoção)
    for (auto& [id, img] : icones_entidades) {
        auto tr_ptr = reg->obter<transformacao>(id);
        if (tr_ptr) { // Garante que a transformação ainda existe
            auto vet = cam.mundoParaTela(tr_ptr->posicao);
            img->m_estilo.m_limites.x = vet.x - img->m_estilo.m_limites.z / 2;
            img->m_estilo.m_limites.y = vet.y - img->m_estilo.m_limites.w / 2;
            img->m_estilo.m_ativo = (vet.z >= 0); // Ativa/desativa baseado na profundidade
        } else {
            // Este caso só deve acontecer se houver um bug onde o icone_entidades não foi limpo.
            // Para robustez, podemos desativar o ícone ou removê-lo aqui também.
            img->m_estilo.m_ativo = false; 
        }
    }
}
/**
 * @brief Atualiza o estado do editor em cada frame.
 * Gerencia inputs, atualiza as caixas da UI, sincroniza a lista de entidades
 * e posiciona os ícones de entidades no viewport.
 */

void sistema_editor::atualizar() {
    chamarInputs();

    atualizarGizmo();

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
