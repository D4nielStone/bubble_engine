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
 * @file texto.cpp
 */

#include "nucleo/engine.hpp"
#include "elementos/texto.hpp"
using namespace becommons::elementos;
using namespace becommons;
            
texto::texto(const std::string frase,
                    becommons::cor color,
                    const unsigned int escala,
                    const flags_texto flags,
                    const std::string path_fonte) : m_texto_frase(frase)
                , m_texto_fonte(path_fonte)
                , m_texto_escala(escala)
                , m_texto_flags(flags)
            {
                m_estilo.m_largura = obterLargura(frase);
                m_estilo.m_altura = obterAltura(frase);
                m_estilo.m_cor_fundo.a = 1;
                m_shader = (std::make_unique<shader>("imagem.vert", "texto.frag"));
            }
texto::texto(const std::string frase,
                    const unsigned int escala,
                    const flags_texto flags,
                    const std::string path_fonte) : m_texto_frase(frase)
                , m_texto_fonte(path_fonte)
                , m_texto_escala(escala)
                , m_texto_flags(flags)
            {
                m_estilo.m_largura = obterLargura(frase);
                m_estilo.m_altura = obterAltura(frase);
                m_estilo.m_cor_fundo.a = 1;
                m_shader = (std::make_unique<shader>("imagem.vert", "texto.frag"));
            }
texto::texto(std::string* frase,
                    const unsigned int escala,
                    const flags_texto flags,
                    const std::string path_fonte) : m_texto_frase_ptr(frase)
                , m_texto_fonte(path_fonte)
                , m_texto_escala(escala)
                , m_texto_flags(flags)
            {
                m_estilo.m_largura = obterLargura(*frase);
                m_estilo.m_cor_fundo.a = 1;
                m_shader = (std::make_unique<shader>("imagem.vert", "texto.frag"));
            }
            
// retorna a largura da linha mais longa dentro de 'frase'
float texto::obterLargura(const std::string& frase) {
                if (m_texto_frase_ptr) m_texto_frase = *m_texto_frase_ptr;
                auto& caracteres = gerenciadorFontes::obterInstancia()
                                       .obter(m_texto_fonte, m_texto_escala);
                if (caracteres.empty()) {
                     return 0.0f;
                 }
              
                 float maxLargura = 0.0f;
                 float larguraLinha = 0.0f;
              
                 for (char32_t c : frase) {
                     if (c == '\n') {
                         // finaliza a linha atual, atualiza o máximo e reseta
                         maxLargura = std::max(maxLargura, larguraLinha);
                         larguraLinha = 0.0f;
                         continue;
                     }
                    auto it = caracteres.find(c);
                    if (it != caracteres.end()) {
                        const caractere& ch = it->second;
                        larguraLinha += (ch.avanco >> 6);
                    }
                }
                // compara a última linha (caso não termine em '\n')
                maxLargura = std::max(maxLargura, larguraLinha);
                return maxLargura;
            }   

            
// retorna a altura total considerando quantas linhas existem em 'frase'
float texto::obterAltura(const std::string& frase) {
                if (m_texto_frase_ptr) m_texto_frase = *m_texto_frase_ptr;
                auto& caracteres = gerenciadorFontes::obterInstancia()
                                       .obter(m_texto_fonte, m_texto_escala);
                if (caracteres.empty()) {
                    return 0.0f;
                }
            
                // conta quantas quebras de linha existem → número de linhas = quebras + 1
                size_t numQuebras = 0;
                for (char32_t c : frase) {
                    if (c == '\n') {
                        ++numQuebras;
                    }
                }
                size_t numLinhas = numQuebras + 1;
            
                // precisa de um método no gerenciador para expor a "altura de linha"
                // por exemplo: gerenciadorFontes::obterAlturaLinha(fonte, escala)
                float alturaLinha = m_texto_escala * 1.4;
            
                return alturaLinha * static_cast<float>(numLinhas);
            }
            
// retorna a altura total considerando quantas linhas existem em 'frase'
float texto::obterAltura(const std::string& frase) {
                if (m_texto_frase_ptr) m_texto_frase = *m_texto_frase_ptr;
                auto& caracteres = gerenciadorFontes::obterInstancia()
                                       .obter(m_texto_fonte, m_texto_escala);
                if (caracteres.empty()) {
                    return 0.0f;
                }
            
                // conta quantas quebras de linha existem → número de linhas = quebras + 1
                size_t numQuebras = 0;
                for (char32_t c : frase) {
                    if (c == '\n') {
                        ++numQuebras;
                    }
                }
                size_t numLinhas = numQuebras + 1;
            
                // precisa de um método no gerenciador para expor a "altura de linha"
                // por exemplo: gerenciadorFontes::obterAlturaLinha(fonte, escala)
                float alturaLinha = m_texto_escala * 1.4;
            
                return alturaLinha * static_cast<float>(numLinhas);
            }
void texto::desenhar(unsigned int ret_VAO) {
                limites_iniciais = m_estilo.m_limites;

                const auto& chs = gerenciadorFontes::obterInstancia().obter(m_texto_fonte, m_texto_escala);
                float y_linha = m_texto_escala, x_inicial = m_estilo.m_limites.x;
                float x_linha = m_estilo.m_limites.x; 
                if((static_cast<uint32_t>(m_texto_flags) & static_cast<uint32_t>(elementos::flags_texto::alinhamento_central))!=0) {
                    x_linha += m_estilo.m_limites.z / 2 - obterLargura(m_texto_frase)/2;
                }
                if(m_texto_frase_ptr) m_texto_frase = *m_texto_frase_ptr;
            
                for(const auto& ca : m_texto_frase) {
                    if (chs.empty()) return;
                    if(ca == '\n') {y_linha += m_texto_escala; x_linha = x_inicial; continue;}
                    caractere ch;
                    if(chs.find(ca) != chs.end()) ch = chs.at(ca);
                    else continue;
            
                    float xpos = x_linha + ch.apoio.x;
                    float ypos = limites_iniciais.y - ch.apoio.y + y_linha;
                    float w = ch.tamanho.x;
                    float h = ch.tamanho.y;

                    m_estilo.m_limites = {xpos, ypos, w, h};
                    m_material.definirTextura("text", {ch.id, ""});
                    caixa::desenhar(ret_VAO);
                    x_linha += (ch.avanco >> 6);
                }
                if (m_texto_frase_ptr) {
                    m_estilo.m_largura = obterLargura(m_texto_frase);
                    m_estilo.m_altura = obterAltura(m_texto_frase);
                }
                m_estilo.m_limites = {limites_iniciais.x, limites_iniciais.y, obterLargura(m_texto_frase), m_estilo.m_altura};
            };
        };
