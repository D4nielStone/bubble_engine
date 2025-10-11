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
#include "elementos/painel.hpp"
using namespace becommons;

container::container(bool floating) { 
    m_estilo.m_corte = true;
    m_estilo.m_flag_estilo = flag_estilo::nenhuma;
    m_floating = floating;
}
header* container::gerarHeader() {
    return nullptr;
}
void container::unsplit(bool keepLeft) {
    if (!m_dividiu || !m_a || !m_b) return;
    m_dividiu = false;

    container* kept = keepLeft ? m_b : m_a;
    container* floating = keepLeft ? m_a : m_b;

    // 1: devo herdar os filhos/containers do kept
    container* a = kept->m_a;
    container* b = kept->m_b;    
    if(a && b) {
        a->m_container_pai = this;
        b->m_container_pai = this;
    }
    m_a = a;
    m_b = b;
    
    for (auto& f : kept->m_filhos) {
        f->m_pai = this;
        adicionar(f);
    }
    if(kept->m_popup_close && kept->m_filhos.size() > 1) {
        motor::obter().m_editor->ui->remover(kept->m_popup_close);
        // remover botão
        m_filhos.erase(m_filhos.begin() + 1);
        gerarHeader();
    }
    m_estilo.m_orientacao_modular = kept->m_estilo.m_orientacao_modular;
    m_porcao = kept->m_porcao;
    m_dividiu = kept->m_dividiu;
    // 2: tornar floating solto
    // O floating fica independente
    floating->m_dividiu = false;
    floating->m_container_pai = nullptr;
    floating->m_floating = true;
    if(floating->m_popup_close && floating->m_filhos.size() > 1) {
        motor::obter().m_editor->ui->remover(floating->m_popup_close);
        floating->m_filhos.erase(floating->m_filhos.begin() + 1);
        floating->gerarHeader();
    }
    floating->m_estilo.m_limites = {
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
    m_filhos.clear();
    auto* a = adicionar<container>();
    auto* b = adicionar<container>();    

    m_estilo.m_orientacao_modular = o;
    m_porcao = std::clamp(porcao_inicial, 0.0f, 1.0f);
    m_dividiu = true;
    return {a, b};
}
        
void container::atualizar() {
    if (m_dividiu) { 
        m_porcao = std::clamp(m_porcao, 0.0f, 1.0f);
        float pb = 1.0f - m_porcao;

        float px = m_estilo.m_limites.x;
        float py = m_estilo.m_limites.y;
        float pw = m_estilo.m_limites.z; // largura
        float ph = m_estilo.m_limites.w; // altura
        if (m_estilo.m_orientacao_modular == estilo::orientacao::horizontal) {
            // coluna A | B  (divide largura)
            m_filhos[0]->m_estilo.m_limites = { px, py, pw * m_porcao, ph };                      // esquerda/topo do split
            m_filhos[1]->m_estilo.m_limites = { px + pw * m_porcao, py, pw * pb, ph };     // direita
        } else {
            // linha A // B  (divide altura)
            m_filhos[0]->m_estilo.m_limites = { px, py, pw, ph * m_porcao };                      // topo
            m_filhos[1]->m_estilo.m_limites = { px, py + ph * m_porcao, pw, ph * pb };     // bottom
        }
    } else
    if (!m_filhos.empty()) {
        m_filhos[0]->m_estilo.m_limites.x = m_estilo.m_limites.x;
        m_filhos[0]->m_estilo.m_limites.y = m_estilo.m_limites.y;
        m_filhos[0]->m_estilo.m_limites.z = std::lerp(m_filhos[0]->m_estilo.m_limites.z, m_estilo.m_limites.z,motor::obter().m_tempo->obterDeltaTime()*15);
        m_filhos[0]->m_estilo.m_limites.w = std::lerp(m_filhos[0]->m_estilo.m_limites.w, m_estilo.m_limites.w,motor::obter().m_tempo->obterDeltaTime()*15);
    }
    caixa::atualizar();
}
