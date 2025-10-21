/** \copyright 
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
 * @file area_de_texto.cpp
 */

#include <filesystem>
#include <string>
#include "becommons/becommons.hpp"
#include "elementos/painel.hpp"
using namespace becommons;

class tab : public elementos::area_de_toque {
public:
    int m_tab_id = 0;
    header* m_header;
    elementos::texto* m_text;
    tab(header* m_header, const std::string& text = "tab") : m_header(m_header), area_de_toque() {
        adicionar<caixa>()->m_estilo.m_cor_fundo = m_header->m_cor_ativado;
        adicionar<caixa>()->m_estilo.m_cor_fundo = m_header->m_cor_ativado;
        m_estilo.m_padding = {1, 1};
        m_text = adicionar<elementos::texto>(text, 10);
    }
    void atualizar() override {
        /* texto */
        if (m_header== nullptr) return;
        m_estilo.m_limites.z = m_text->obterLargura(m_text->m_texto_frase) + m_header->m_tab_h*2 + m_estilo.m_padding.x*4;
        m_estilo.m_limites.w = m_header->m_tab_h;
        m_filhos[2]->m_estilo.m_limites.z = m_estilo.m_limites.z;
        m_filhos[2]->m_estilo.m_limites.x = m_estilo.m_limites.x + m_header->m_tab_h + m_estilo.m_padding.x;
        m_filhos[2]->m_estilo.m_limites.y = m_estilo.m_limites.y + m_estilo.m_padding.y;
        m_filhos[2]->m_estilo.m_limites.w = m_header->m_tab_h;
        /* barra de baixo */
        m_filhos[0]->m_estilo.m_raio = 1.f;
        m_filhos[0]->m_estilo.m_limites.x = m_estilo.m_limites.x;
        m_filhos[0]->m_estilo.m_limites.y = m_estilo.m_limites.y + m_header->m_tab_h - 4;
        m_filhos[0]->m_estilo.m_limites.z = m_estilo.m_limites.z;
        m_filhos[0]->m_estilo.m_limites.w = 4;
        /* caixa de dentro */
        m_filhos[1]->m_estilo.m_raio = 3.f;
        m_filhos[1]->m_estilo.m_limites.x = m_estilo.m_limites.x + m_header->m_tab_h / 2 - 3;
        m_filhos[1]->m_estilo.m_limites.y = m_estilo.m_limites.y + m_header->m_tab_h / 2 - 3;
        m_filhos[1]->m_estilo.m_limites.z = 6;
        m_filhos[1]->m_estilo.m_limites.w = 6;
    
        area_de_toque::atualizar();
    }
};

painel::painel(const std::string& label) : label(label) {
    m_estilo.m_orientacao_modular = estilo::orientacao::vertical;
    m_estilo.m_cor_fundo = cor(0.10f);
    m_estilo.m_cor_borda = cor(0.13f);
    m_estilo.m_padding_geral = {1, 1};
}
void painel::atualizar() {
    bool mouse = motor::obter().m_inputs->obter(inputs::MOUSE_E) ||
        motor::obter().m_inputs->obter(inputs::MOUSE_D)          ||
        motor::obter().m_inputs->obter(inputs::MOUSE_MEIO);
    if(     mouse 
            && mouseEmCima() && m_estilo.m_ativo
            && trigger == false ) m_selecionado = true;
    if(     mouse 
            && (mouseEmCima() == false||m_estilo.m_ativo==false)
            && trigger == false ) {
        m_selecionado = false;
    }
    if(mouse) trigger = true;
    else trigger = false;
    
    m_estilo.m_cor_borda = cor(0.13f);
    caixa::atualizar();
}
header::~header() {
    for(auto& tab : m_tabs) delete tab;
    m_tabs.clear();
}
header::header() {
    m_estilo.m_flag_estilo = flag_estilo::nenhuma;
    m_estilo.m_cor_borda = cor(0.13f);
    m_estilo.m_cor_fundo = cor(0.07f);
}
void header::desenhar(unsigned int vao) {
    caixa::desenhar(vao);

    for(auto& tab : m_tabs) {
        tab->m_projecao = m_projecao;
        tab->desenhar(vao);
        for(auto& filho : tab->m_filhos) {
            filho->m_projecao = m_projecao;
            filho->desenhar(vao);
        }
    }
}
void header::atualizar() {
    // -- se tabs mudou
    if (m_tabs.size() != m_filhos.size()) {
        for(auto& tab : m_tabs) delete tab;
        m_tabs.clear();

        // -- add
        for(auto& c : m_filhos) {
            // -- obtém o painel
            auto p = static_cast<painel*>(c.get());
            // -- add a tab
            m_tabs.push_back(new tab(this, p->label));
            m_tabs.back()->m_tab_id = m_tabs.size() - 1; // -- set id
        }
    }

    // -- cursor para posicionamento
    float cursor = m_estilo.m_limites.x + 1;
    size_t idx = 0;
    // -- lógica de toque
    for(auto& tab : m_tabs) {
        cursor += m_estilo.m_padding_geral.x + tab->m_estilo.m_padding.x;
        tab->m_estilo.m_limites.x = cursor;
        tab->m_estilo.m_limites.y = m_estilo.m_limites.y;
        cursor += tab->m_estilo.m_limites.z;
        if(tab->pressionado()) tab_atual = idx;
        idx++;
    }

    // -- painel
    for(size_t i = 0; i < m_filhos.size(); i++) {
        if(i!=tab_atual) {
            m_tabs[i]->m_estilo.m_cor_fundo = cor(0.03);
            m_tabs[i]->m_filhos[0]->m_estilo.m_cor_fundo.a = 0;
            m_tabs[i]->m_filhos[1]->m_estilo.m_cor_fundo.a = 0;
            m_filhos[i]->m_estilo.m_ativo = false;
        } else {
            m_tabs[i]->m_estilo.m_cor_fundo = cor(0.07);
            m_tabs[i]->m_filhos[0]->m_estilo.m_cor_fundo = m_cor_ativado;
            m_tabs[i]->m_filhos[1]->m_estilo.m_cor_fundo = m_cor_ativado;
            m_filhos[i]->m_estilo.m_ativo = true;
        }
        m_filhos[i]->m_estilo.m_limites = m_estilo.m_limites;
        m_filhos[i]->m_estilo.m_limites.y += m_tab_h;
        m_filhos[i]->m_estilo.m_limites.w -= m_tab_h;
        m_filhos[i]->atualizar();
    }
    for(auto& tab : m_tabs) {
        tab->atualizar();
        for(auto& filho : tab->m_filhos) {
            filho->atualizar();
        }
    }
}
            
paineis::entity::entity() : painel("entities") {
    auto* context = adicionar<caixa>();
    context->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_percentual;
    context->m_estilo.m_altura = 1;
    context->m_estilo.m_largura = 1;
    
    auto* barra_lateral = context->adicionar<caixa>();
    barra_lateral->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_percentual;
    barra_lateral->m_estilo.m_largura = 1;
    barra_lateral->m_estilo.m_cor_fundo = cor(0.08f);
    barra_lateral->m_estilo.m_orientacao_modular = estilo::orientacao::vertical;
    barra_lateral->m_estilo.m_padding_geral = {2, 2};
    auto* barra_lateral_d = context->adicionar<caixa>();
    barra_lateral_d->m_estilo.m_flag_estilo |= flag_estilo::largura_justa | flag_estilo::altura_percentual;
    barra_lateral_d->m_estilo.m_cor_fundo = cor(0.07f);
    barra_lateral_d->m_estilo.m_orientacao_modular = estilo::orientacao::vertical;
    barra_lateral_d->m_estilo.m_padding_geral = {2, 2};
}
void paineis::entity::atualizar() {
    if(!motor::obter().m_levelmanager || !motor::obter().m_levelmanager->obterFaseAtual() || motor::obter().m_levelmanager->carregando()) {
        m_filhos[0]->m_filhos[0]->m_filhos.clear();
        m_filhos[0]->m_filhos[1]->m_filhos.clear();
        old_contage = 0;
        return;
    }
    auto reg = motor::obter().m_levelmanager->obterFaseAtual()->obterRegistro();
    
    if (old_contage != reg->entidades.size()) {
        old_contage = reg->entidades.size();
    motor::obter().fila_opengl.push([this, reg]() {
        m_filhos[0]->m_filhos[0]->m_filhos.clear();
        m_filhos[0]->m_filhos[1]->m_filhos.clear();
        for(auto& [id, comps] : reg->entidades) {
            m_filhos[0]->m_filhos[0]->adicionar<elementos::botao>([this, id]() {
                motor::obter().m_editor->m_entidade_selecionada = id;
            }, "entity." + std::to_string(id), "cube.png", 10); 
            m_filhos[0]->m_filhos[1]->adicionar<elementos::botao>([reg, id]() {
                reg->remover(id);
            }, "", "remover.png", 16); 
        }
    });
    }
    painel::atualizar();
}

paineis::inspector::inspector() : painel("inspector") {
}

paineis::jogo::jogo() : painel("jogo") {
    framebuffer = adicionar<elementos::imagem>(0, true);
    framebuffer->m_estilo.m_flag_estilo = flag_estilo::modular | flag_estilo::largura_percentual | flag_estilo::alinhamento_central | flag_estilo::altura_percentual;
    framebuffer->m_estilo.m_largura = 1;
    framebuffer->m_estilo.m_altura = 1;
    if(motor::obter().m_renderer->cameras.empty() == false) {
    auto cam = *motor::obter().m_renderer->cameras.begin();
    cam->viewport_ptr = &framebuffer->m_estilo.m_limites; // Associa o viewport da câmera aos limites do framebuffer
    framebuffer->id = cam->textura;
    }
    auto* lateral = framebuffer->adicionar<caixa>();
    lateral->m_estilo.m_cor_fundo = {0.07, 0.07, 0.07, 0.5};
    lateral->m_estilo.m_padding_geral = {2,2};
    lateral->m_estilo.m_flag_estilo |= flag_estilo::altura_justa | flag_estilo::largura_justa;
    lateral->adicionar<elementos::botao>([this](){
            play();
            }, "", "play.png", 18);
}
            
void paineis::jogo::play(){
    motor::obter().m_levelmanager->obterFaseAtual()->iniciar();
    framebuffer->m_estilo.m_flag_estilo = flag_estilo::modular | flag_estilo::largura_percentual | flag_estilo::alinhamento_fim | flag_estilo::altura_percentual;
    m_filhos[0]->m_filhos[0]->m_filhos.clear();
    m_filhos[0]->m_filhos[0]->adicionar<elementos::botao>([this](){
            stop();
            }, "", "stop.png", 18);
}
void paineis::jogo::pause(){
}
void paineis::jogo::stop(){
    motor::obter().m_levelmanager->obterFaseAtual()->parar();
    framebuffer->m_estilo.m_flag_estilo = flag_estilo::modular | flag_estilo::largura_percentual | flag_estilo::alinhamento_central | flag_estilo::altura_percentual;
    m_filhos[0]->m_filhos[0]->m_filhos.clear();
    m_filhos[0]->m_filhos[0]->adicionar<elementos::botao>([this](){
            play();
            }, "", "play.png", 18);
}

void paineis::jogo::atualizar() {
    if(motor::obter().m_renderer->cameras.empty() == false) {
    auto cam = *motor::obter().m_renderer->cameras.begin();
    cam->ativarFB();
    cam->viewport_ptr = &framebuffer->m_estilo.m_limites; // Associa o viewport da câmera aos limites do framebuffer
    framebuffer->m_material.definirTextura("textura", {cam->textura , ""});
    }
    painel::atualizar();
}
            
paineis::editor::editor(camera_editor* cam) : painel("editor"), cam(cam) {
    framebuffer = adicionar<elementos::imagem>(cam->textura, true);
    framebuffer->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_percentual;
    framebuffer->m_estilo.m_largura = 1;
    framebuffer->m_estilo.m_altura = 1;
    cam->viewport_ptr = &framebuffer->m_estilo.m_limites; // Associa o viewport da câmera aos limites do framebuffer
    auto* lateral = framebuffer->adicionar<caixa>();
    lateral->m_estilo.m_cor_fundo = {0.07, 0.07, 0.07, 0.5};
    lateral->m_estilo.m_padding_geral = {2,2};
    lateral->m_estilo.m_orientacao_modular = estilo::orientacao::vertical;
    lateral->m_estilo.m_flag_estilo |= flag_estilo::altura_justa | flag_estilo::largura_justa;
    cam->atualizarMovimentacao(); // Atualiza a posição e orientação da câmera
    sistema_renderizacao::calcularTransformacao(cam->transform); // Recalcula a matriz de transformação da câmera
}

void paineis::editor::atualizar() {
    if(m_selecionado) {
        cam->atualizarMovimentacao(); // Atualiza a posição e orientação da câmera
        sistema_renderizacao::calcularTransformacao(cam->transform); // Recalcula a matriz de transformação da câmera
        auto reg = motor::obter().m_levelmanager->obterFaseAtual()->obterRegistro();
        if (motor::obter().m_inputs->obter(inputs::MOUSE_D)) {
            // picking
            dvet2 mouse = motor::obter().m_inputs->obterMousePos();
            raio ray = cam->pontoParaRaio({static_cast<int>(mouse.x), static_cast<int>(mouse.y)});
            resultadoRaio resultado = motor::obter().m_fisica->emitirRaio(ray);

                reg->cada<fisica, renderizador>([&](const uint32_t entidade){
                    auto comp = reg->obter<fisica>(entidade);
                    auto compB = reg->obter<renderizador>(entidade);
                    if(resultado.atingiu && comp->m_corpo_rigido == resultado.objetoAtingido) {
                        motor::obter().m_editor->m_entidade_selecionada = entidade;
                        compB->m_outline = true;
                    } else {
                        compB->m_outline = false;
                    }
                });
        }
      /*if (motor::obter().m_inputs->obter(inputs::CTRL_E) &&
            motor::obter().m_inputs->obter(inputs::)
                ) {
        }*/
    }
    painel::atualizar();
}
paineis::file_manager::file_manager(const std::string& base_dir) : m_directory(base_dir), painel("files") {
    auto* context = adicionar<caixa>();
    context->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_percentual;
    context->m_estilo.m_altura = 1;
    context->m_estilo.m_largura = 1;
    context->m_estilo.m_padding_geral = {2, 2};
    auto* barra_lateral = context->adicionar<caixa>();
    barra_lateral->m_estilo.m_flag_estilo |= flag_estilo::altura_percentual;
    barra_lateral->m_estilo.m_cor_fundo = cor(0.07f);
    barra_lateral->m_estilo.m_cor_borda = cor(0.2f);
    barra_lateral->m_estilo.m_altura = 1;
    barra_lateral->m_estilo.m_largura = 150;
    barra_lateral->m_estilo.m_orientacao_modular = estilo::orientacao::vertical;
    barra_lateral->m_estilo.m_padding_geral = {5, 2};
    auto* barra_central = context->adicionar<caixa>();
    barra_central->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_percentual;
    barra_central->m_estilo.m_cor_fundo = cor(0.07f);
    barra_central->m_estilo.m_cor_borda = cor(0.2f);
    barra_central->m_estilo.m_altura = 1;
    barra_central->m_estilo.m_largura = 1;
    barra_central->m_estilo.m_padding_geral = {5, 2};
    research(m_directory);
}
void paineis::file_manager::research(const std::string& dir) {
    int scl = 10;
    auto* barra_lateral = m_filhos[0]->m_filhos[0].get();
    barra_lateral->m_filhos.clear();

    if (motor::obter().m_projeto) {
        for(auto entry : std::filesystem::directory_iterator(dir)) {
            auto path = entry.path();
            if (std::filesystem::is_directory(entry.path())) {
                auto str = entry.path().string();
                auto filename = entry.path().filename().string();
                if (filename[0] == '.') continue;

                auto* btn = barra_lateral->adicionar<elementos::botao>(
                    [this, str]() {
                        motor::obter().fila_opengl.push([this, str] {
                            research(str);
                        });
                    },
                    filename,
                    "folder.png",
                    scl
                );
            } else
            if (std::filesystem::is_regular_file(entry.path())) {
                auto str = entry.path().string();
                auto filename = entry.path().filename().string();
                if (filename[0] == '.') continue;
                if (entry.path().extension().string() == ".lua") {
                auto* btn = barra_lateral->adicionar<elementos::botao>(
                    [this, str]() {
                    },
                    filename,
                    "Codigo.png",
                    scl
                );
                }
            }
        }
        auto str = std::filesystem::path(dir).parent_path().string();

        if(std::filesystem::equivalent(m_directory, dir)) return;
        auto* btn = barra_lateral->adicionar<elementos::botao>(
            [this, str]() {
                motor::obter().fila_opengl.push([this, str] {
                    research(str);
                });
            },
            "..",
            "folder.png",
            scl
        );
    }
}
            
void paineis::file_manager::pick_file(const std::string& filtro, std::function<void(const std::string)>&& funcao) {
    m_filhos.clear();
    auto* text = adicionar<elementos::texto>("aqruivos encontrados para o filtro " + filtro + ":", 12, elementos::flags_texto::alinhamento_central);
    text->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual;
    text->m_estilo.m_largura = 1;
    auto* barra_central = adicionar<caixa>();
    barra_central->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_percentual;
    barra_central->m_estilo.m_cor_fundo = cor(0.07f);
    barra_central->m_estilo.m_cor_borda = cor(0.2f);
    barra_central->m_estilo.m_altura = 1;
    barra_central->m_estilo.m_largura = 1;
    barra_central->m_estilo.m_padding_geral = {5, 1};
    barra_central->m_estilo.m_orientacao_modular = estilo::orientacao::vertical;

    auto f_reload = [this, filtro, barra_central, funcao](){
    barra_central->m_filhos.clear();
    for (const auto& entry : std::filesystem::recursive_directory_iterator(m_directory)) {
        if (entry.is_regular_file() && entry.path().extension() == filtro) {
            auto str = entry.path().string();
            barra_central->adicionar<elementos::botao>(
                [funcao, str]() mutable {
                    funcao(str);
                },
                entry.path().filename().string(),
                "Codigo.png",
                10
            );
        }
    }
    };
    f_reload();
    // caixa de texto
    auto* c = adicionar<elementos::caixa_de_texto>("Novo arquivo " + filtro);
    auto* btnB = adicionar<elementos::botao>([this, f_reload, c, filtro](){
        /**/
            auto s_buffer_c = c->obterBuffer();
            bool hasSpecialChar = false;
            std::string specialChars = R"(/\!@#$%^&*().)";
            for (char c : s_buffer_c) {
                if (specialChars.find(c) != std::string::npos) {
                    hasSpecialChar = true;
                }
            }
            if(s_buffer_c.empty() || hasSpecialChar) {
                depuracao::emitir(erro, "caixa_de_texto", "nome inválido.");
                return;
            }
            auto dir = motor::obter().m_projeto->m_diretorio + "/codigos/" + s_buffer_c + filtro;
            std::ofstream ofs(dir);
            if (ofs.is_open()) {
                ofs.close();
                depuracao::emitir(info, "painel", "arquivo " + dir + " criado.");
                f_reload();
            }
        /**/
            }, "criar");
    btnB->m_estilo.m_padding_geral = {2, 2};
    c->m_estilo.m_cor_fundo = cor(0.07f);
    c->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual;
    c->m_estilo.m_largura = 1;
    btnB->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual;
    btnB->m_estilo.m_largura = 1;
}

void paineis::file_manager::atualizar() {
    caixa::atualizar();
}
            
paineis::text_editor::text_editor(const std::string& file) : painel(std::filesystem::path(file).filename().string()) {
    std::ifstream ifs(file);
    if(ifs.is_open() == false) return;
    std::string fileContent(
        (std::istreambuf_iterator<char>(ifs)),
        std::istreambuf_iterator<char>()
    );
    ifs.close();
    m_estilo.m_orientacao_modular = estilo::orientacao::horizontal;
    ct = adicionar<elementos::caixa_de_texto>("escreva seu codigo aqui.", fileContent);
    ct->m_estilo.m_flag_estilo = flag_estilo::altura_percentual | flag_estilo::largura_percentual | flag_estilo::modular;
    ct->m_estilo.m_cor_fundo = cor(0.07f);
    ct->m_break = true;
    auto* barra_num = adicionar<caixa>();
    barra_num->m_estilo.m_flag_estilo = flag_estilo::altura_percentual | flag_estilo::largura_justa | flag_estilo::modular;
    barra_num->m_estilo.m_orientacao_modular = estilo::orientacao::vertical;
    barra_num->m_estilo.m_cor_fundo = cor(0.09f);
    barra_num->m_estilo.m_padding.y = 2;
    barra_num->m_estilo.m_altura = 1;
    txt = barra_num->adicionar<elementos::texto>("1", 10);
}
void paineis::text_editor::atualizar() {
    auto buffer = ct->obterBuffer();

    // get number of lines base on \n char to add text 
    txt->m_texto_frase = "";
    int num_lines = 1; // começa com 1, mesmo que não haja '\n'
    if(buffer.empty() == false) {
    for (char c : buffer) {
        if (c == '\n') {
            txt->m_texto_frase += std::to_string(num_lines);
            txt->m_texto_frase += '\n';
            ++num_lines;
        }
    }
    }
    txt->m_texto_frase += std::to_string(num_lines);
    caixa::atualizar();
}

paineis::coding::coding() : painel("coding") {
    m_estilo.m_orientacao_modular = estilo::orientacao::horizontal;
    auto* context = adicionar<caixa>();
    context->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_percentual | flag_estilo::alinhamento_central;
    context->m_estilo.m_altura = 1;
    context->m_estilo.m_largura = 1;
    context->m_estilo.m_padding_geral = {0, 10};
    context->m_estilo.m_cor_fundo = cor(0.07f);
    context->m_estilo.m_cor_borda = cor(0.2f);
    auto* contextB = adicionar<caixa>();
    contextB->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_percentual;
    contextB->m_estilo.m_altura = 1;
    contextB->m_estilo.m_largura = 1;
    contextB->m_estilo.m_padding_geral = {0, 0};
    contextB->m_estilo.m_cor_fundo = cor(0.07f);
    contextB->m_estilo.m_cor_borda = cor(0.2f);
    auto* files = contextB->adicionar<container>();
    files->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_percentual;
    files->m_estilo.m_altura = 1;
    files->m_estilo.m_largura = 1;

    auto* btn = context->adicionar<elementos::botao>([](){
            }, "Importar Arquivo", "lua.png", 22);

    // create popup for filemanager
    auto f = [this](const std::string result){
            open(result);
            };
    auto* pop = motor::obter().m_editor->ui->novo_popup(btn);
    auto* p = pop->adicionar<file_manager>(motor::obter().m_projeto->m_diretorio);
    p->pick_file(".lua", f);
    p->m_estilo.m_largura = 300;
    p->m_estilo.m_altura = 300;

    btn->m_estilo.m_cor_fundo = cor(0.07f);
    btn->m_estilo.m_padding_geral = {2, 2};
}
void paineis::coding::open(const std::string file) {
    auto* files = static_cast<container*>(m_filhos[1]->m_filhos[0].get());
    files->nova_tab<text_editor>(file);
}
void paineis::coding::atualizar() {
    caixa::atualizar();
}
