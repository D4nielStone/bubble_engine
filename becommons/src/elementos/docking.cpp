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

container::container() { 
    m_estilo.m_flag_estilo = flag_estilo::nenhuma;
}
        
void container::unsplit() {
    dividiu = false;
    m_containers.clear();
}
        
std::pair<container*, container*>  container::split(float porcao_inicial, estilo::orientacao o) {
    if(dividiu) return{nullptr, nullptr}; // -- evita duplicatas
    auto* a = motor::obter().m_editor->ui->adicionar<container>();
    auto* b = motor::obter().m_editor->ui->adicionar<container>();    
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
