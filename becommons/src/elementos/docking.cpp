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

#include "becommons/becommons.hpp"
using namespace becommons;

painel::painel(const std::string& label) : label(label) {
    m_estilo.m_orientacao_modular = estilo::orientacao::vertical;
    m_estilo.m_cor_fundo = cor(0.10f);
    m_estilo.m_cor_borda = cor(0.13f);
    m_estilo.m_padding_geral = {1, 1};
}
header::header(const std::string& label) {
    m_estilo.m_flag_estilo |= flag_estilo::altura_justa;
    m_estilo.m_largura = 1;
    m_estilo.m_cor_fundo = cor(0.065f);
    m_estilo.m_cor_borda = cor(0.32f);
    m_estilo.m_padding_geral = {2, 2};
}
void header::atualizar() {
    caixa::atualizar();
}
container::container() { 
    m_estilo.m_flag_estilo = flag_estilo::nenhuma;
}
        
void container::unsplit() {
    /*if(filho) {
        filho->unsplit();
        for(auto* tab : filho->m_filhos) {
            remover_tab(tab);
        }
        filho->m_filhos.clear();
        filho.reset();
        filho = nullptr;
        dividiu = false;
    }*/
}
        
std::pair<container*, container*>  container::split(float porcao_inicial, estilo::orientacao o) {
    m_filhos.clear();
    auto* a = adicionar<container>();
    auto* b = adicionar<container>();    

    m_estilo.m_orientacao_modular = o;
    porcao = std::clamp(porcao_inicial, 0.0f, 1.0f);
    dividiu = true;
    return {a, b};
}
        
void container::atualizar() {
    if (dividiu) { 
        porcao = std::clamp(porcao, 0.0f, 1.0f);
        float pb = 1.0f - porcao;

        float px = m_estilo.m_limites.x;
        float py = m_estilo.m_limites.y;
        float pw = m_estilo.m_limites.z; // largura
        float ph = m_estilo.m_limites.w; // altura
        if (m_estilo.m_orientacao_modular == estilo::orientacao::horizontal) {
            // coluna A | B  (divide largura)
            m_filhos[0]->m_estilo.m_limites = { px, py, pw * porcao, ph };                      // esquerda/topo do split
            m_filhos[1]->m_estilo.m_limites = { px + pw * porcao, py, pw * pb, ph };     // direita
        } else {
            // linha A // B  (divide altura)
            m_filhos[0]->m_estilo.m_limites = { px, py, pw, ph * porcao };                      // topo
            m_filhos[1]->m_estilo.m_limites = { px, py + ph * porcao, pw, ph * pb };     // bottom
        }
    } else
    if (!m_filhos.empty()) {
        m_filhos[tab_atual]->m_estilo.m_limites = becommons::lerp(m_filhos[tab_atual]->m_estilo.m_limites, m_estilo.m_limites,motor::obter().m_tempo->obterDeltaTime()*10);
    }
    caixa::atualizar();
}
void paineis::entity::atualizar() {
    if(!motor::obter().m_levelmanager || !motor::obter().m_levelmanager->obterFaseAtual() || motor::obter().m_levelmanager->carregando()) return;
    auto reg = motor::obter().m_levelmanager->obterFaseAtual()->obterRegistro();
    
    if (old_contage != reg->entidades.size()) {
        old_contage = reg->entidades.size();
        registro_atualizou = true;
    }

    if (registro_atualizou) {
        m_filhos.clear();
        for(auto& [id, comps] : reg->entidades) {
            adicionar<elementos::botao>([&, id]() {
                entidade_selecionada = id;
                std::cout << "entidade selecionada:" << id <<"\n";
            }, "entity." + std::to_string(id), "cube.png"); 
            registro_atualizou = false;
        }
    }
    caixa::atualizar();
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
