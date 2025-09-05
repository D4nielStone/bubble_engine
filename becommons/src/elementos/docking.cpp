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
#include "elementos/docking.hpp"
using namespace becommons;

container::container() { 
    m_estilo.m_flag_estilo = flag_estilo::nenhuma;
}
        
void container::unsplit(bool keepLeft) {
    if (!dividiu || m_containers.size() != 2) return;
    dividiu = false;

    int keep = keepLeft ? 1 : 0;
    int floatingSide = keepLeft ? 0 : 1;

    container* kept = m_containers[keep];
    container* floating = m_containers[floatingSide];
    m_containers.clear();

    // 1: devo herdar os filhos/containers do kept
    if(kept->dividiu) {
        container* a = kept->m_containers[0];
        container* b = kept->m_containers[1];    
        a->father = this;
        b->father = this;
        m_containers.push_back(a);
        m_containers.push_back(b);
    
        m_estilo.m_orientacao_modular = kept->m_estilo.m_orientacao_modular;
        porcao = kept->porcao;
        this->ladoA = kept->ladoA;
        dividiu = kept->dividiu;
    } else {
        for (auto& f : kept->m_filhos) {
            f->m_pai = this;
            f->configurar();
            adicionar(std::move(f));
        }
        if(kept->m_popup) {motor::obter().m_editor->ui->remover(kept->m_popup);
        if(father) {
            m_popup = motor::obter().m_editor->ui->adicionar<elementos::popup>(static_cast<elementos::botao*>(m_filhos[1].get()));
            m_popup->adicionar<elementos::botao>([this](){
                father->unsplit(ladoA);
            }, "unpin");
        }
        }
    }
    // 2: tornar floating solto
    // O floating fica independente
    floating->father = nullptr;
    floating->floating = true;
    floating->limite_flutuando = {
        floating->m_estilo.m_limites.x + floating->m_estilo.m_limites.z/4,
        floating->m_estilo.m_limites.y + floating->m_estilo.m_limites.w/4,
        floating->m_estilo.m_limites.z/2,
        floating->m_estilo.m_limites.w/2
    };

    // 3: limpar referência interna
    // O container do antigo kept deve ser deletado da loop
    motor::obter().m_editor->ui->remover(kept);
}
 
std::pair<container*, container*>  container::split(float porcao_inicial, estilo::orientacao o) {
    if(dividiu) return{nullptr, nullptr}; // -- evita duplicatas
    
    // gera doi novos containers
    auto* a = motor::obter().m_editor->ui->adicionar<container>();
    auto* b = motor::obter().m_editor->ui->adicionar<container>();    
    // determina seu lado a ou b
    a->ladoA= true;
    b->ladoA= false;
    a->father = this;
    b->father = this;
    m_containers.push_back(a);
    m_containers.push_back(b);

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
            m_containers[0]->m_estilo.m_limites = { px, py, pw * porcao, ph };                      // esquerda/topo do split
            m_containers[1]->m_estilo.m_limites = { px + pw * porcao, py, pw * pb, ph };     // direita
        } else {
            // linha A // B  (divide altura)
            m_containers[0]->m_estilo.m_limites = { px, py, pw, ph * porcao };                      // topo
            m_containers[1]->m_estilo.m_limites = { px, py + ph * porcao, pw, ph * pb };     // bottom
        }
    } else
    if (!m_filhos.empty()) {
        if(floating) m_estilo.m_limites = limite_flutuando;
        // m_filhos[0] = header
        // m_filhos[0]->m_filhos = paineis
        m_filhos[0]->m_estilo.m_limites = becommons::lerp(m_filhos[0]->m_estilo.m_limites, m_estilo.m_limites,motor::obter().m_tempo->obterDeltaTime()*10);
        float w = 16.f;
        m_filhos[1]->m_estilo.m_limites = becommons::lerp(m_filhos[1]->m_estilo.m_limites, fvet4{
            m_estilo.m_limites.x + m_estilo.m_limites.z - w,
            m_estilo.m_limites.y,
            w,
            w},motor::obter().m_tempo->obterDeltaTime()*10);
        m_filhos[1]->m_filhos[0]->m_estilo.m_limites = m_filhos[1]->m_estilo.m_limites;
    }
    caixa::atualizar();
}
