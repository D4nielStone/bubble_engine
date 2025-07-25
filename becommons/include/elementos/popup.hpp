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
 * @file popup.hpp
 */

#pragma once
#include "becommons_namespace.hpp"
#include "area_de_toque.hpp"
#include <memory>
#include <cmath>

namespace BECOMMONS_NS {
    namespace elementos {
        class popup : public area_de_toque {
            private:
                botao* m_referencia {nullptr};
                bool prevMouseE = false;
                int x_antigo = 0;
                int y_antigo = 0;
                float m_velocidade_lerp = 13.f;
            public:

                popup() {
                    m_estilo.m_orientacao_modular = estilo::orientacao::vertical;
                    m_estilo.m_ativo = false;
                }
                void configurar() override {
                    if (m_pai && m_pai->tipo() == tipo_caixa::botao)
                        m_referencia = static_cast<botao*>(m_pai);
                    m_estilo.m_cor_fundo = cor(0.1f);
                    m_estilo.m_cor_borda = cor(0.07f);
                    m_estilo.m_flag_estilo |= flag_estilo::largura_justa | flag_estilo::altura_justa;
                    m_estilo.m_padding_geral = {3, 3};
                    m_estilo.m_ativo = false;
                }
                void atualizar() override {
                    if (!m_referencia) return;

                    // Detecta borda de clique
                    bool mouseE = motor::obter().m_inputs->obter(inputs::MOUSE_E);
                    bool mousePressedEdge = (mouseE && !prevMouseE);
                    prevMouseE = mouseE;

                    // Se for clique único...
                    if (mousePressedEdge) {
                        if (!m_estilo.m_ativo) {
                            if (m_referencia->pressionado()) {
                                m_estilo.m_ativo = true;
                                // captura posição inicial do cursor
                                x_antigo = inputs::obterMousePos().x;
                                y_antigo = inputs::obterMousePos().y;
                            }
                        } else {
                            if (!mouseEmCima()) {
                                m_estilo.m_ativo = false;
                            }
                        }
                    } 

                    float dt = motor::obter().m_tempo->obterDeltaTime();
                    x_antigo = m_referencia->pressionado() ? inputs::obterMousePos().x : x_antigo;
                    y_antigo = m_referencia->m_pressionado ? inputs::obterMousePos().y : y_antigo;
                    float mais_largo = 20.f, cursorx = 0.f;
                    float mais_alto = 20.f, cursory = 0.f;
                    cursorx = m_estilo.m_padding_geral.x;
                    cursory = m_estilo.m_padding_geral.y;
                    bool quebra = false;
                    // Ativado
                    if (m_estilo.m_ativo) {
                    float cursor_x = m_estilo.m_padding_geral.x;
                    float cursor_y = m_estilo.m_padding_geral.y;
                    float max_largura_total = 0.f;
                    float max_altura_total = 0.f;
                    
                    if (m_estilo.m_orientacao_modular == estilo::orientacao::vertical) {
                        float largura_coluna_atual = 0.f;
             
                        for (auto& filho : m_filhos) {
                            // Posiciona o filho (exemplo, não está no seu código original mas é necessário)
                            // filho->definirPosicao(cursor_x, cursor_y);
             
                            // Atualiza as dimensões da coluna/container
                            largura_coluna_atual = std::max(largura_coluna_atual, filho->m_estilo.m_limites.z);
                            cursor_y += filho->m_estilo.m_limites.w + m_estilo.m_padding_geral.y;
                            max_altura_total = std::max(max_altura_total, cursor_y);
             
                            // Verifica a quebra de linha IMEDIATAMENTE
                            if (filho->tem(flag_estilo::quebrar_linha)) {
                                // Avança para a próxima coluna
                                cursor_x += largura_coluna_atual + m_estilo.m_padding_geral.x*2;
                                
                                // Atualiza a largura total necessária
                                max_largura_total = std::max(max_largura_total, cursor_x);
                                
                                // Reseta a altura do cursor e a largura da nova coluna
                                cursor_y = m_estilo.m_padding_geral.y;
                                largura_coluna_atual = 0.f;
                            }
                        }
                        // Garante que a última coluna seja contabilizada na largura total
                        max_largura_total = std::max(max_largura_total + m_estilo.m_padding_geral.x, cursor_x + largura_coluna_atual + m_estilo.m_padding_geral.x);
             
                        // Atualiza os limites do container (ex: m_estilo.m_limites.z = max_largura_total)
             
                    } else { // Horizontal
                        float altura_linha_atual = 0.f;
             
                        for (auto& filho : m_filhos) {
                            // ... lógica similar ...
                            altura_linha_atual = std::max(altura_linha_atual, filho->m_estilo.m_limites.w);
                            cursor_x += filho->m_estilo.m_limites.z + m_estilo.m_padding_geral.x;
                            max_largura_total = std::max(max_largura_total, cursor_x);
             
                            if (filho->tem(flag_estilo::quebrar_linha)) {
                                cursor_y += altura_linha_atual + m_estilo.m_padding_geral.y*2;
                                max_altura_total = std::max(max_altura_total, cursor_y);
                                cursor_x = m_estilo.m_padding_geral.x;
                                altura_linha_atual = 0.f;
                            }
                        }
                        max_altura_total = std::max(max_altura_total + m_estilo.m_padding_geral.y, cursor_y + altura_linha_atual + m_estilo.m_padding_geral.y);
                    }
                    // Atualiza domenções
                    m_estilo.m_limites.z = std::lerp(m_estilo.m_limites.z, max_largura_total, m_velocidade_lerp * dt);
                    m_estilo.m_limites.w = std::lerp(m_estilo.m_limites.w, max_altura_total, m_velocidade_lerp * dt);

                    bool pode_aparecer_direita = (x_antigo + m_estilo.m_limites.z) < janela::obterInstancia().tamanho.x;
                    bool pode_aparecer_abaixo = (y_antigo + m_estilo.m_limites.w) < janela::obterInstancia().tamanho.y;

                    float novo_x = pode_aparecer_direita ? x_antigo : x_antigo - m_estilo.m_limites.z;
                    float novo_y = pode_aparecer_abaixo ? y_antigo : y_antigo - m_estilo.m_limites.w ;
                    m_estilo.m_limites.x = std::lerp(m_estilo.m_limites.x, novo_x, m_velocidade_lerp * dt);
                    m_estilo.m_limites.y = std::lerp(m_estilo.m_limites.y, novo_y, m_velocidade_lerp * dt);
                } else {
                        // Desativado
                        m_estilo.m_limites = {m_referencia->m_estilo.m_limites.x, m_referencia->m_estilo.m_limites.y, 20, 20};
                    }
                }
                tipo_caixa tipo() const override {
                    return tipo_caixa::popup;
                }
        };
    }
}
