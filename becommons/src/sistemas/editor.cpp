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
#include "api/mat.hpp"
#include "elementos/barra_menu.hpp"
#include "sistemas/editor.hpp"
#include <filesystem>
#include <typeinfo>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

using namespace becommons;
using namespace beeditor;
class header;
class painel : public caixa {
public:
    bool callback_remocao {false};
    std::string label;
    painel(const std::string& label = "panel") : label(label) {
        m_estilo.m_orientacao_modular = estilo::orientacao::vertical;
        m_estilo.m_cor_fundo = cor(0.10f);
        m_estilo.m_cor_borda = cor(0.13f);
        m_estilo.m_padding_geral = {2, 2};
        adicionar<header>(label);
    }
};

/** \class header: 
 *  \brief ´header´ é uma caixa que funciona como a aba de uma janela. Pode ser em abas ou janela única.
 */
class header : public caixa {
public:
    header(const std::string& label) {
        m_estilo.m_flag_estilo |= flag_estilo::altura_justa;
        m_estilo.m_largura = 1;
        m_estilo.m_cor_fundo = cor(0.065f);
        m_estilo.m_cor_borda = cor(0.32f);
        m_estilo.m_padding_geral = {2, 2};
        adicionar<elementos::texto>(label)->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual;
        adicionar<elementos::botao>([this](){
            static_cast<painel*>(m_pai)->callback_remocao = true;
            motor::obter().fila_opengl.push([this](){
                m_pai->remover(this);
                m_pai->m_pai->remover(m_pai);
            });
        }, "", "remover.png");
    }

    void atualizar() override {
        caixa::atualizar();
        auto texto = m_filhos[0]->m_estilo.m_limites.z  + m_estilo.m_padding_geral.x*2 + m_filhos[1]->m_estilo.m_limites.z;
        auto tudo = m_pai->m_estilo.m_limites.z - m_pai->m_estilo.m_padding_geral.x*2;
        m_estilo.m_largura = std::max(tudo, texto);
    }
};

class container : public caixa {
private:
    std::vector<painel*> m_tabs;
    unsigned int tab_atual{0};
    std::shared_ptr<container> filho{nullptr};
    container* pai {nullptr};
    float porcao = 0.5f;
    bool dividiu{false};
    fvet4 bounds; // limites do container dividido
public:
    container() {
        m_estilo.m_flag_estilo = flag_estilo::nenhuma;
    }
    void unsplit() {
        if(filho) {
            std::cout <<"US\n";
            filho->unsplit();
            for(auto* tab : filho->m_tabs) {
                remover_tab(tab);
            }
            filho->m_tabs.clear();
            filho.reset();
            filho = nullptr;
            dividiu = false;
        }
    }
    container* split(float porcao_inicial = 0.5f, estilo::orientacao o = estilo::orientacao::horizontal) {
        m_estilo.m_orientacao_modular = o;
        porcao = std::clamp(porcao_inicial, 0.0f, 1.0f);
        filho = std::make_shared<container>();
        filho->pai = this;
        dividiu = true;
        return filho.get();
    }

    void tab(painel* p) {
        m_tabs.push_back(p);
        tab_atual = m_tabs.size() - 1;
    }
    void remover_tab(painel* p) {
    auto it = std::find(m_tabs.begin(), m_tabs.end(), p);
    if (it != m_tabs.end()) {
            m_tabs.erase(it);
            if (m_tabs.empty() && pai) {
                    // se container ficou vazio, desmonta split
            pai->unsplit();
            } else {
                tab_atual = std::min(tab_atual, (unsigned int)(m_tabs.size() - 1));
            }
        }
        if (p->m_pai) {
            p->m_pai->remover(p); // remove da hierarquia visual
        }
    }

    void recurssividade() {
        porcao = std::clamp(porcao, 0.0f, 1.0f);
        float pb = 1.0f - porcao;

        const fvet4* referencia = nullptr;
        if (pai) {
            referencia = &bounds;
        } else {
            referencia = &m_estilo.m_limites; // fallback (root absoluto)
        }

        float px = referencia->x;
        float py = referencia->y;
        float pw = referencia->z; // largura
        float ph = referencia->w; // altura

        if (dividiu && filho) { 
            if (filho->m_tabs.empty()) {
                unsplit();
                return;
            }
            if (m_estilo.m_orientacao_modular == estilo::orientacao::horizontal) {
                // coluna A | B  (divide largura)
                bounds = { px, py, pw * porcao, ph };                      // esquerda/topo do split
                filho->bounds = { px + pw * porcao, py, pw * pb, ph };     // direita
            } else {
                // linha A // B  (divide altura)
                bounds = { px, py, pw, ph * porcao };                      // topo
                filho->bounds = { px, py + ph * porcao, pw, ph * pb };     // bottom
            }
            // atualiza recursivamente o filho (se existir)
            filho->recurssividade();
        } else {
            bounds = *referencia;
        }
        
        if (!m_tabs.empty()) {
        // remove tabs inválidas
        auto it = std::remove_if(m_tabs.begin(), m_tabs.end(),
                [](painel* ptr) {
                    return ptr->callback_remocao;
                });
        if (it != m_tabs.end()) {
            m_tabs.erase(it, m_tabs.end());
            if (m_tabs.empty()) {
                return; // não há mais tabs
            }
            tab_atual = std::min(tab_atual, (unsigned int)(m_tabs.size() - 1));
        }
    
        // só acessa se ainda houver tabs válidas
        m_tabs[tab_atual]->m_estilo.m_limites = becommons::lerp(m_tabs[tab_atual]->m_estilo.m_limites, bounds,motor::obter().m_tempo->obterDeltaTime()*3);
    }
}
};

namespace paineis {
    class file_manager : public painel {
    private:
    public:
        file_manager() : painel("file manager") {
            m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_percentual;
            m_estilo.m_largura = 1;
            m_estilo.m_altura = 0.5;
        }
        void atualizar() override {
        }
    };
    class entity : public painel {
    private:
        bool registro_atualizou = true;
        uint32_t entidade_selecionada;
        size_t old_contage = 0;
    public:
        entity() : painel("entity register") {
            m_estilo.m_flag_estilo |= flag_estilo::largura_justa | flag_estilo::altura_percentual;
            m_estilo.m_altura = 1;
        }
        uint32_t obter() const {
            return entidade_selecionada;
        }
        void atualizar() override {
            if(!motor::obter().m_levelmanager || !motor::obter().m_levelmanager->obterFaseAtual() || motor::obter().m_levelmanager->carregando()) return;
            auto reg = motor::obter().m_levelmanager->obterFaseAtual()->obterRegistro();
            
            if (old_contage != reg->entidades.size()) {
                old_contage = reg->entidades.size();
                registro_atualizou = true;
            }

            if (registro_atualizou) {
                m_filhos.clear();
                adicionar<header>(label);
                for(auto& [id, comps] : reg->entidades) {
                    adicionar<elementos::botao>([&, id]() {
                        entidade_selecionada = id;
                    }, "entity." + std::to_string(id), "cube.png"); 
                    registro_atualizou = false;
                }
            }
        }
    };
    class inspector : public painel {
    public:
        inspector() : painel("inspector editor") {
            auto* context = adicionar<caixa>();
            context->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_percentual;
            context->m_estilo.m_altura = 1;
            context->m_estilo.m_largura = 1;
            
            m_estilo.m_flag_estilo |= flag_estilo::largura_justa | flag_estilo::altura_percentual;
            m_estilo.m_altura = 1;
            m_estilo.m_padding_geral.x = 0;
            auto* barra_lateral = context->adicionar<caixa>();
            barra_lateral->m_estilo.m_flag_estilo |= flag_estilo::largura_justa | flag_estilo::altura_percentual;
            barra_lateral->m_estilo.m_cor_fundo = cor(0.07f);
            barra_lateral->m_estilo.m_orientacao_modular = estilo::orientacao::vertical;
            barra_lateral->m_estilo.m_padding_geral = {2, 2};
            barra_lateral->adicionar<elementos::botao>(nullptr, "", "cube.png");
            barra_lateral->adicionar<elementos::botao>(nullptr, "", "Fisica.png");
            barra_lateral->adicionar<elementos::botao>(nullptr, "", "Renderizador.png");

            auto* btn_add_comp = context->adicionar<elementos::botao>([](){}, "add component");
            btn_add_comp->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual;
            btn_add_comp->m_estilo.m_largura = 1;
            btn_add_comp->m_estilo.m_cor_borda = cor(0.4f);
            btn_add_comp->m_estilo.m_cor_fundo = cor(0.06f);
        }
    };
    class editor : public painel {
    public:
        elementos::imagem* framebuffer;
        editor(camera* cam) : painel("editor view") {
            m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_percentual;
            m_estilo.m_largura = 1;
            m_estilo.m_altura = 1;
            framebuffer = adicionar<elementos::imagem>(cam->textura, true);
            framebuffer->m_estilo.m_flag_estilo = flag_estilo::largura_percentual | flag_estilo::altura_percentual;
            framebuffer->m_estilo.m_largura = 1;
            framebuffer->m_estilo.m_altura = 1;
            cam->viewport_ptr = &framebuffer->m_estilo.m_limites; // Associa o viewport da câmera aos limites do framebuffer
        }
    };
};

bool gatilho_= false;

// Declaração global para acesso rápido às caixas de entidades e inspetor.
// Essas caixas são gerenciadas pela UI e contêm os elementos visuais
// para manipulação de entidades e seus componentes.
paineis::entity* p_entidades; 
paineis::inspector* p_inspetor;
paineis::editor* p_editor;
paineis::file_manager* p_files;
container* dock;

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
void sistema_editor::adicionarCaixas() {
    // Configuração da caixa raiz (root UI element)
    ui->m_raiz->m_estilo.m_orientacao_modular = estilo::orientacao::vertical;

    // Configuração da seção superior da UI (barra de menu e versão)
    auto* menu = ui->m_raiz->adicionar<custom::barra_menu>();
    menu->adicionar_botao(" File ", [](){});
    menu->adicionar_botao("Edit ", [](){});
    menu->adicionar_botao("View ", [](){});
    menu->adicionar_botao("Help ", [](){});

    // Configurando ambiente com containers
    // O container principal [dock] irá preencher a tela a-baixo da menubar
    dock = ui->m_raiz->adicionar<container>();
    dock->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_percentual;
    dock->m_estilo.m_largura = 1;
    dock->m_estilo.m_altura = 1;
    
    // Criação de paineis
    // Não importa de que container o painel é filho, apenas que ele seja inserido no loop da interface.
    // Isto pois os containers não precisam usar o vetor m_filhos, apenas usar suas referências na segmentação.
    p_entidades = dock->adicionar<paineis::entity>();
    p_editor = dock->adicionar<paineis::editor>(cam.get());
    //p_files = dock->adicionar<paineis::file_manager>();
    p_inspetor = dock->adicionar<paineis::inspector>();
    
    dock->tab(p_entidades);
    auto* meio = dock->split(0.2);
    meio->tab(p_editor);
    auto* fim = meio->split(0.6);
    fim->tab(p_inspetor);
 }
/**
 * @brief Salva as configurações da câmera e do estado do editor em um arquivo JSON.
 * Cria o diretório 'usr' se não existir e serializa os dados da câmera.
 */
void sistema_editor::salvarEditor() {
    auto _usr = motor::obter().m_projeto->m_diretorio + "/usr";
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
 * @brief Gerencia os inputs do usuário para ações do editor.
 * Inclui atalhos para salvar, rodar o runtime, criar/remover entidades e salvar fases.
 * Também atualiza a movimentação da câmera do editor.
 */
void sistema_editor::chamarInputs() {
    if(!motor::obter().m_levelmanager || !motor::obter().m_levelmanager->obterFaseAtual() || motor::obter().m_levelmanager->carregando()) return;
    // Salva o editor e o projeto, e inicia o runtime
    // Gerencia inputs com a tecla CTRL pressionada para evitar repetições
    if(motor::obter().m_inputs->obter(inputs::E_CTRL)) {
        if(motor::obter().m_inputs->obter(inputs::R)) {
            salvarEditor();
            motor::obter().m_levelmanager->salvarTudo();
            //executarRuntime();
        } 
        else if(gatilho_ && motor::obter().m_inputs->obter(inputs::F3)) {
            if(motor::obter().m_levelmanager->obterFaseAtual()->rodando)
                motor::obter().m_levelmanager->obterFaseAtual()->parar();
            else
                motor::obter().m_levelmanager->obterFaseAtual()->iniciar();
            gatilho_ = false;
        }
        else if(gatilho_ && motor::obter().m_inputs->obter(inputs::A)) {
            motor::obter().m_levelmanager->obterFaseAtual()->obterRegistro()->criar(); // Cria nova entidade
            gatilho_ = false;
        } else if(gatilho_ && motor::obter().m_inputs->obter(inputs::X)) {
            motor::obter().m_levelmanager->obterFaseAtual()->obterRegistro()->remover(p_entidades->obter()); // Remove a entidade selecionada
            gatilho_ = false;
        } else if(gatilho_ && motor::obter().m_inputs->obter(inputs::S)) {
            salvarEditor(); // Salva configurações do editor
            motor::obter().m_levelmanager->salvarTudo(); // Salva todas as fases do projeto
            gatilho_ = false;
        } else if(!motor::obter().m_inputs->obter(inputs::R) && !motor::obter().m_inputs->obter(inputs::F3) && !motor::obter().m_inputs->obter(inputs::X) && !motor::obter().m_inputs->obter(inputs::A) && !motor::obter().m_inputs->obter(inputs::S)) {
            gatilho_ = true; // Reinicia o gatilho quando nenhuma das teclas é pressionada
        }
    }
    cam->atualizarMovimentacao(); // Atualiza a posição e orientação da câmera
    sistema_renderizacao::calcularTransformacao(cam->transform); // Recalcula a matriz de transformação da câmera
    if(m_salvar_ao_fechar && motor::obter().m_janela->deveFechar()) {
        salvarEditor(); // Salva o editor se a janela estiver fechando e a flag estiver ativa
    }
}
        
/**
 * @brief Atualiza os icones de interface interativos
 * com o editor.
 */
void sistema_editor::atualizarGizmo() {
}
/**
 * @brief Atualiza o estado do editor em cada frame.
 * Gerencia inputs, atualiza as caixas da UI, sincroniza a lista de entidades
 * e posiciona os ícones de entidades no viewport.
 */

void sistema_editor::atualizar() {
    chamarInputs();
    ui->atualizar();
    dock->recurssividade();
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
    adicionarCaixas(); // Constrói a interface do editor

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
    auto _usr = proj->m_diretorio + "/usr";
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
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Aguarda 500ms antes da próxima verificação
    }
}*/
