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
    header* m_header;
    tab(header* m_header, const std::string& text = "tab") : m_header(m_header), area_de_toque() {
        adicionar<caixa>()->m_estilo.m_cor_fundo = m_header->m_cor_ativado;
        adicionar<caixa>()->m_estilo.m_cor_fundo = m_header->m_cor_ativado;
        m_estilo.m_padding = {1, 1};
        adicionar<elementos::texto>(text, 12);
    }
    void atualizar() override {
        /* texto */
        m_estilo.m_limites.z = m_header->m_tab_l;
        m_estilo.m_limites.w = m_header->m_tab_h;
        m_filhos[2]->m_estilo.m_limites.z = m_header->m_tab_l;
        m_filhos[2]->m_estilo.m_limites.x = m_estilo.m_limites.x + m_header->m_tab_h + m_estilo.m_padding.x;
        m_filhos[2]->m_estilo.m_limites.y = m_estilo.m_limites.y + m_estilo.m_padding.y;
        m_filhos[2]->m_estilo.m_limites.w = m_header->m_tab_h;
        /* barra de baixo */
        m_filhos[0]->m_estilo.m_limites.x = m_estilo.m_limites.x;
        m_filhos[0]->m_estilo.m_limites.y = m_estilo.m_limites.y + m_header->m_tab_h - 2;
        m_filhos[0]->m_estilo.m_limites.z = m_estilo.m_limites.z;
        m_filhos[0]->m_estilo.m_limites.w = 2;
        /* caixa de dentro */
        m_filhos[1]->m_estilo.m_limites.x = m_estilo.m_limites.x + m_header->m_tab_h / 2 - 2;
        m_filhos[1]->m_estilo.m_limites.y = m_estilo.m_limites.y + m_header->m_tab_h / 2 - 2;
        m_filhos[1]->m_estilo.m_limites.z = 4;
        m_filhos[1]->m_estilo.m_limites.w = 4;
    
        area_de_toque::atualizar();
    }
};

painel::painel(const std::string& label) : label(label) {
    m_estilo.m_orientacao_modular = estilo::orientacao::vertical;
    m_estilo.m_cor_fundo = cor(0.10f);
    m_estilo.m_cor_borda = cor(0.13f);
    m_estilo.m_padding_geral = {1, 1};
}
header::header() {
    m_estilo.m_flag_estilo = flag_estilo::nenhuma;
    m_estilo.m_cor_fundo = cor(0.072f);
    m_estilo.m_cor_borda = cor(0.065f);
}
void header::atualizar() {
    if (m_tabs.size() != m_filhos.size()) {
        for(auto& tab : m_tabs) {
            motor::obter().m_editor->ui->remover(m_tabs.back().get());
            tab.reset();
        }
        m_tabs.clear();
        for(auto& c : m_filhos) {
            auto p = static_cast<painel*>(c.get());
            m_tabs.push_back(std::make_unique<tab>(this, p->label));
            motor::obter().m_editor->ui->inserir(m_tabs.back().get());
        }
    }

    float cursor = m_estilo.m_limites.x + 2;
    size_t idx = 0;
    for(auto& tab : m_tabs) {
        cursor += m_estilo.m_padding_geral.x + tab->m_estilo.m_padding.x;
        tab->m_estilo.m_limites.x = cursor;
        tab->m_estilo.m_limites.y = m_estilo.m_limites.y;
        cursor += tab->m_estilo.m_limites.z;
        if(tab->pressionado()) tab_atual = idx;
        idx++;
    }

    // painel
    for(size_t i = 0; i < m_filhos.size(); i++) {
        if(i!=tab_atual) {
            m_tabs[i]->m_estilo.m_cor_fundo = cor(0.07);
            m_tabs[i]->m_filhos[0]->m_estilo.m_cor_fundo = m_cor_desativado;
            m_tabs[i]->m_filhos[1]->m_estilo.m_cor_fundo = m_cor_desativado;
            m_filhos[i]->m_estilo.m_ativo = false;
        } else {
            m_tabs[i]->m_estilo.m_cor_fundo = cor(0.1);
            m_tabs[i]->m_filhos[0]->m_estilo.m_cor_fundo = m_cor_ativado;
            m_tabs[i]->m_filhos[1]->m_estilo.m_cor_fundo = m_cor_ativado;
            m_filhos[i]->m_estilo.m_ativo = true;
        }
        m_filhos[i]->m_estilo.m_limites = m_estilo.m_limites;
        m_filhos[i]->m_estilo.m_limites.y += m_tab_h;
        m_filhos[i]->m_estilo.m_limites.w -= m_tab_h;
        m_filhos[i]->atualizar();
        m_tabs[i]->atualizar();
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
    if(!motor::obter().m_levelmanager || !motor::obter().m_levelmanager->obterFaseAtual() || motor::obter().m_levelmanager->carregando()) return;
    auto reg = motor::obter().m_levelmanager->obterFaseAtual()->obterRegistro();
    
    if (old_contage != reg->entidades.size()) {
        old_contage = reg->entidades.size();
    motor::obter().fila_opengl.push([this, reg]() {
        m_filhos[0]->m_filhos[0]->m_filhos.clear();
        m_filhos[0]->m_filhos[1]->m_filhos.clear();
        for(auto& [id, comps] : reg->entidades) {
            m_filhos[0]->m_filhos[0]->adicionar<elementos::botao>([this, id]() {
                entidade_selecionada = id;
            }, "entity." + std::to_string(id), "cube.png", 12); 
            m_filhos[0]->m_filhos[1]->adicionar<elementos::botao>([reg, id]() {
                reg->remover(id);
            }, "", "remover.png", 16); 
        }
    });
    }
}

paineis::inspector::inspector() : painel("inspector") {
}
            
paineis::editor::editor(camera* cam) : painel("editor") {
    framebuffer = adicionar<elementos::imagem>(cam->textura, true);
    framebuffer->m_estilo.m_flag_estilo = flag_estilo::largura_percentual | flag_estilo::altura_percentual;
    framebuffer->m_estilo.m_largura = 1;
    framebuffer->m_estilo.m_altura = 1;
    cam->viewport_ptr = &framebuffer->m_estilo.m_limites; // Associa o viewport da câmera aos limites do framebuffer
}
paineis::file_manager::file_manager() : painel("files") {
    auto* context = adicionar<caixa>();
    context->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_percentual;
    context->m_estilo.m_altura = 1;
    context->m_estilo.m_largura = 1;
    context->m_estilo.m_padding_geral = {2, 2};

    auto* barra_lateral = context->adicionar<caixa>();
    barra_lateral->m_estilo.m_flag_estilo |= flag_estilo::largura_justa | flag_estilo::altura_percentual;
    barra_lateral->m_estilo.m_cor_fundo = cor(0.07f);
    barra_lateral->m_estilo.m_cor_borda = cor(0.2f);
    barra_lateral->m_estilo.m_altura = 1;
    barra_lateral->m_estilo.m_orientacao_modular = estilo::orientacao::vertical;
    barra_lateral->m_estilo.m_padding_geral = {5, 2};
    auto* barra_central = context->adicionar<caixa>();
    barra_central->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_percentual;
    barra_central->m_estilo.m_cor_fundo = cor(0.07f);
    barra_central->m_estilo.m_cor_borda = cor(0.2f);
    barra_central->m_estilo.m_altura = 1;
    barra_central->m_estilo.m_largura = 1;
    barra_central->m_estilo.m_padding_geral = {5, 2};
    if (motor::obter().m_projeto)
    research(motor::obter().m_projeto->m_diretorio);
    else
    research(obterDiretorioHome());
}
void paineis::file_manager::research(const std::string& dir) {
    static int scl = 12;
    auto* barra_lateral = m_filhos[0]->m_filhos[0].get();
    barra_lateral->m_filhos.clear();
    if (motor::obter().m_projeto) {
        for(auto entry : std::filesystem::directory_iterator(dir)) {
            auto path = entry.path();
            if (std::filesystem::is_regular_file(entry.path())) {
                if (entry.path().extension() == ".fase") {
                    auto* pop = barra_lateral->adicionar<elementos::botao>(
                            nullptr,
                            entry.path().filename(),
                            "scene.png",
                            scl)->adicionar<elementos::popup>(false);
                    pop->adicionar<elementos::botao>([path]() {
                            }, "import", "abrir.png", scl);
                } else
                if (entry.path().extension() == ".lua") {
                    auto* pop = barra_lateral->adicionar<elementos::botao>(
                            nullptr,
                            entry.path().filename(),
                            "lua.png",
                            scl)->adicionar<elementos::popup>(false);
                    pop->adicionar<elementos::botao>([path]() {
                            }, "import", "abrir.png", scl);
                } else
                if (entry.path().extension() == ".py") {
                    auto* pop = barra_lateral->adicionar<elementos::botao>(
                            nullptr,
                            entry.path().filename(),
                            "python.png",
                            scl)->adicionar<elementos::popup>(false);
                    pop->adicionar<elementos::botao>([path]() {
                            }, "import", "abrir.png", scl);
                } else
                if (entry.path().extension() == ".obj"||
                    entry.path().extension() == ".fbx"||
                    entry.path().extension() == ".dae") {
                    auto* pop = barra_lateral->adicionar<elementos::botao>(
                            nullptr,
                            entry.path().filename(),
                            "cubo_branco",
                            scl)->adicionar<elementos::popup>(false);
                    pop->adicionar<elementos::botao>([path]() {
                            }, "import", "abrir.png", scl);
                } else
                if (entry.path().extension() == ".json" ||
                    entry.path().extension() == ".bubble" ) {
                    auto* pop = barra_lateral->adicionar<elementos::botao>(
                            nullptr,
                            entry.path().filename(),
                            "tool.png",
                            scl)->adicionar<elementos::popup>(false);
                    pop->adicionar<elementos::botao>([path]() {
                            }, "remove", "", scl);
                    pop->adicionar<elementos::botao>([path]() {
                            }, "copy", "", scl);
                } 
            } else 
            {
                    auto* pop = barra_lateral->adicionar<elementos::botao>(
                            nullptr,
                            entry.path().filename(),
                            "folder.png",
                            scl)->adicionar<elementos::popup>(false);
                    pop->adicionar<elementos::botao>(nullptr, "remove", "", scl);
                    pop->adicionar<elementos::botao>(nullptr, "copy", "", scl);
            }
        }
    }
}
void paineis::file_manager::atualizar() {
    caixa::atualizar();
}

paineis::assets_manager::assets_manager() : painel("assets") {
    auto* context = adicionar<caixa>();
    context->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_percentual;
    context->m_estilo.m_altura = 1;
    context->m_estilo.m_largura = 1;
    context->m_estilo.m_padding_geral = {2, 2};

    auto* barra_lateral = context->adicionar<caixa>();
    barra_lateral->m_estilo.m_flag_estilo |= flag_estilo::largura_justa | flag_estilo::altura_percentual;
    barra_lateral->m_estilo.m_cor_fundo = cor(0.07f);
    barra_lateral->m_estilo.m_cor_borda = cor(0.2f);
    barra_lateral->m_estilo.m_altura = 1;
    barra_lateral->m_estilo.m_orientacao_modular = estilo::orientacao::vertical;
    barra_lateral->m_estilo.m_padding_geral = {5, 2};
    auto* barra_central = context->adicionar<caixa>();
    barra_central->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_percentual;
    barra_central->m_estilo.m_cor_fundo = cor(0.07f);
    barra_central->m_estilo.m_cor_borda = cor(0.2f);
    barra_central->m_estilo.m_altura = 1;
    barra_central->m_estilo.m_largura = 1;
    barra_central->m_estilo.m_padding_geral = {5, 2};
    if (motor::obter().m_projeto)
    research(motor::obter().m_projeto->m_diretorio);
    else
    research(obterDiretorioHome());
}
void paineis::assets_manager::research(const std::string& dir) {
}
void paineis::assets_manager::atualizar() {
    caixa::atualizar();
}
