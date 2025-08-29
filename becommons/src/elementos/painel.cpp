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

painel::painel(const std::string& label) : label(label) {
    m_estilo.m_orientacao_modular = estilo::orientacao::vertical;
    m_estilo.m_cor_fundo = cor(0.10f);
    m_estilo.m_cor_borda = cor(0.13f);
    m_estilo.m_padding_geral = {1, 1};
    adicionar<header>(label);
}
header::header(const std::string& label) {
    m_estilo.m_flag_estilo |= flag_estilo::altura_justa;
    m_estilo.m_largura = 1;
    m_estilo.m_cor_fundo = cor(0.072f);
    m_estilo.m_cor_borda = cor(0.065f);
    m_estilo.m_padding_geral = {4, 0};
    adicionar<elementos::texto>(label, 12);
}
void header::atualizar() {
    caixa::atualizar();
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
        m_filhos[1]->m_filhos[0]->m_filhos.clear();
        m_filhos[1]->m_filhos[1]->m_filhos.clear();
        for(auto& [id, comps] : reg->entidades) {
            m_filhos[1]->m_filhos[0]->adicionar<elementos::botao>([this, id]() {
                entidade_selecionada = id;
            }, "entity." + std::to_string(id), "cube.png", 12); 
            m_filhos[1]->m_filhos[1]->adicionar<elementos::botao>([reg, id]() {
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
    research(motor::obter().m_projeto->m_diretorio);
}
void paineis::file_manager::research(const std::string& dir) {
    static int scl = 12;
    auto* barra_lateral = m_filhos[1]->m_filhos[0].get();
    barra_lateral->m_filhos.clear();
    if (motor::obter().m_projeto) {
        for(auto entry : std::filesystem::directory_iterator(dir)) {
            auto path = entry.path();
            if (std::filesystem::is_regular_file(entry.path())) {
                if (entry.path().extension() == ".fase") {
                    barra_lateral->adicionar<elementos::botao>([&]() {
                    }, entry.path().filename(), "scene.png", scl);
                } else
                if (entry.path().extension() == ".lua") {
                    barra_lateral->adicionar<elementos::botao>([&]() {
                    }, entry.path().filename(), "lua.png", scl);
                } else
                if (entry.path().extension() == ".py") {
                    barra_lateral->adicionar<elementos::botao>([&]() {
                    }, entry.path().filename(), "python.png", scl);
                }
                if (entry.path().extension() == ".obj") {
                    barra_lateral->adicionar<elementos::botao>([&]() {
                    }, entry.path().filename(), "Renderizador.png", scl);
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
    if(std::filesystem::exists(dir))
    barra_lateral->adicionar<elementos::botao>([this, dir]() {
    motor::obter().fila_opengl.push([this, dir]() {
        research(std::filesystem::path(dir).parent_path().string());
    });
    }, "back ..");
}
void paineis::file_manager::atualizar() {
}
