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
    tab(const std::string& label) : area_de_toque() {
        adicionar<caixa>()->m_estilo.m_cor_fundo = cor(0.8, 0.66, 0.86);
        adicionar<caixa>()->m_estilo.m_cor_fundo = cor(0.8, 0.66, 0.86);
    }
    void atualizar() override {
        m_filhos[0]->m_estilo.m_limites.x = m_estilo.m_limites.x;
        m_filhos[0]->m_estilo.m_limites.y = m_estilo.m_limites.y + 18;
        m_filhos[0]->m_estilo.m_limites.z = m_estilo.m_limites.z;
        m_filhos[0]->m_estilo.m_limites.w = 2;
        m_filhos[1]->m_estilo.m_limites.x = m_estilo.m_limites.x + 8;
        m_filhos[1]->m_estilo.m_limites.y = m_estilo.m_limites.y + 8;
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
            m_tabs.push_back(std::make_unique<tab>(p->label));
            motor::obter().m_editor->ui->inserir(m_tabs.back().get());
        }
    }

    float cursor = m_estilo.m_limites.x + 2;
    size_t idx = 0;
    for(auto& tab : m_tabs) {
        tab->m_estilo.m_limites.x = cursor;
        tab->m_estilo.m_limites.y = m_estilo.m_limites.y;
        cursor += tab->m_estilo.m_limites.z + 2;
        tab->atualizar();
        if(tab->pressionado()) {
            tab_atual = idx;
        }
        idx++;
    }

    // painel
    for(size_t i = 0; i < m_filhos.size(); i++) {
        if(i!=tab_atual) {
            m_tabs[i]->m_estilo.m_cor_fundo = cor(0.03);
            m_tabs[i]->m_filhos[0]->m_estilo.m_cor_fundo = cor(0.075);
            m_tabs[i]->m_filhos[1]->m_estilo.m_cor_fundo = cor(0.075);
            m_filhos[i]->m_estilo.m_limites = m_estilo.m_limites;
            m_filhos[i]->m_estilo.m_ativo = false;
        } else {
            m_tabs[tab_atual]->m_estilo.m_cor_fundo = cor(0.17);
            m_tabs[tab_atual]->m_filhos[0]->m_estilo.m_cor_fundo = cor(0.8, 0.66, 0.86);
            m_tabs[tab_atual]->m_filhos[1]->m_estilo.m_cor_fundo = cor(0.8, 0.66, 0.86);
            m_filhos[i]->m_estilo.m_ativo = true;
            m_filhos[tab_atual]->m_estilo.m_limites = m_estilo.m_limites;
            m_filhos[tab_atual]->m_estilo.m_limites.y += 20;
            m_filhos[tab_atual]->m_estilo.m_limites.w -= 20;
            m_filhos[i]->atualizar();
        }
    }
}
            
paineis::entity::entity() : painel("entity register") {
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

paineis::inspector::inspector() : painel("inspector editor") {
    auto* context = adicionar<caixa>();
    context->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual | flag_estilo::altura_percentual;
    context->m_estilo.m_altura = 1;
    context->m_estilo.m_largura = 1;
    
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
            
paineis::editor::editor(camera* cam) : painel("editor view") {
    framebuffer = adicionar<elementos::imagem>(cam->textura, true);
    framebuffer->m_estilo.m_flag_estilo = flag_estilo::largura_percentual | flag_estilo::altura_percentual;
    framebuffer->m_estilo.m_largura = 1;
    framebuffer->m_estilo.m_altura = 1;
    cam->viewport_ptr = &framebuffer->m_estilo.m_limites; // Associa o viewport da câmera aos limites do framebuffer
}
paineis::file_manager::file_manager() : painel("file manager") {
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
                    barra_lateral->adicionar<elementos::botao>(nullptr , entry.path().filename(), "scene.png", scl);
                } else
                if (entry.path().extension() == ".lua") {
                    barra_lateral->adicionar<elementos::botao>([&]() {
                    }, entry.path().filename(), "lua.png", scl);
                } else
                if (entry.path().extension() == ".py") {
                    barra_lateral->adicionar<elementos::botao>([&]() {
                    }, entry.path().filename(), "python.png", scl);
                }
                if (entry.path().extension() == ".obj"||
                    entry.path().extension() == ".fbx"||
                    entry.path().extension() == ".dae") {
                    auto* pop = barra_lateral->adicionar<elementos::botao>(nullptr , entry.path().filename(), "Renderizador.png", scl)->adicionar<elementos::popup>(false);
                    pop->adicionar<elementos::botao>([path]() {
                            motor::obter().importarModelo(path);
                            }, "import to scene", "abrir.png", 12);
                }
                if (entry.path().extension() == ".json") {
                    barra_lateral->adicionar<elementos::botao>([&]() {
                    }, entry.path().filename(), "tool.png", scl);
                }
            } else {
                barra_lateral->adicionar<elementos::botao>([this, path]() {
                motor::obter().fila_opengl.push([this, path]() {
                    research(path);
                });
                }, entry.path().filename(), "folder.png", scl);
            }
        }
    }
    if (!dir.empty())
    if(std::filesystem::exists(dir))
    barra_lateral->adicionar<elementos::botao>([this, dir]() {
    motor::obter().fila_opengl.push([this, dir]() {
        research(std::filesystem::path(dir).parent_path().string());
    });
    }, "back ..");
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
