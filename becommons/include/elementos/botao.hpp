/** @copyright 
MIT License
Copyright (c) 2025 Daniel Oliveira

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
*/
/**
 * @file botao.hpp
 */

#pragma once
#include "namespace.hpp"
#include "imagem.hpp"
#include "texto.hpp"
#include "area_de_toque.hpp"
#include <memory>

namespace BECOMMONS_NS{
    namespace elementos{
        struct botao : area_de_toque
        {
            /// @name Construtores com ponteiro para aparência
            /// @{
            botao(const std::function<void()> &fctn, std::unique_ptr<texto> m_texto)    : area_de_toque(fctn) {
                cfgTexto(m_texto.get());
                m_filhos.push_back(std::move(m_texto));
            }
            botao(const std::function<void()> &fctn, std::unique_ptr<imagem> m_imagem)   : area_de_toque(fctn) {
                cfgImagem(m_imagem.get());
                m_filhos.push_back(std::move(m_imagem));
            }
            botao(bool* ptr, std::unique_ptr<imagem> m_imagem)                           : area_de_toque(ptr) {
                cfgImagem(m_imagem.get());
                m_filhos.push_back(std::move(m_imagem));
            }
            botao(bool* ptr, std::unique_ptr<texto> m_texto)                           : area_de_toque(ptr) {
                cfgTexto(m_texto.get());
                m_filhos.push_back(std::move(m_texto));
            }
            /// @}
            /// @name Construtores de texto simplificado
            /// @{
            botao(const std::function<void()> &fctn, const std::string& txt) : area_de_toque(fctn) {
                auto m_texto = std::make_unique<texto>(txt);
                cfgTexto(m_texto.get());
                m_filhos.push_back(std::move(m_texto));
            }
            botao(bool* ptr, const std::string& txt) : area_de_toque(ptr) {
                auto m_texto = std::make_unique<texto>(txt);
                cfgTexto(m_texto.get());
                m_filhos.push_back(std::move(m_texto));
            }
            /// @}
            ~botao() {
            }
            inline void cfgImagem(imagem* m_imagem) {
                m_flag_estilo = flag_estilo::modular;

                m_imagem->m_altura = m_altura;
                m_imagem->m_largura = m_largura;
                m_imagem->m_flag_estilo = m_flag_estilo;
                m_largura += m_padding_geral.x * 2;
            }
            inline void cfgTexto(texto* m_texto) {
                m_flag_estilo = flag_estilo::modular;
                
                m_largura = m_texto->obterLargura(m_texto->m_texto_frase) + m_padding_geral.x * 2;
                
                m_texto->m_flag_estilo = m_flag_estilo;
            }
            bool pressionado() override {
                m_pressionado = area_de_toque::pressionado();
                m_cor_borda.a = m_mouse_cima ? 1.f : 0.f;
                return m_pressionado;
            }
            void configurar() override {
                for(auto& f : m_filhos) {
                    if(auto img = dynamic_cast<imagem*>(f.get())) {

                        cfgImagem(img);
                    } else
                    if(auto txt = dynamic_cast<texto*>(f.get())) {
                        cfgTexto(txt);
                    }
                }
            }
        };  
    } // elementos
} // bubble
