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
 * @file botao.hpp
 */

#pragma once
#include "imagem.hpp"
#include "texto.hpp"
#include "area_de_toque.hpp"
#include <memory>

namespace becommons{
    namespace elementos{
        class botao : public area_de_toque {
        private:
            float old_fundo_alpha;
        public:
            tipo_caixa tipo() const override { return tipo_caixa::botao; }
            /// @name Construtores com ponteiro para aparência
            /// @{
            botao(const std::function<void()> &fctn, std::unique_ptr<texto> m_texto)    : area_de_toque(fctn) {
                m_filhos.push_back(std::move(m_texto));
            }
            botao(const std::function<void()> &fctn, std::unique_ptr<imagem> m_imagem)   : area_de_toque(fctn) {
                m_filhos.push_back(std::move(m_imagem));
                old_fundo_alpha = m_estilo.m_cor_fundo.a;
            }
            botao(bool* ptr, std::unique_ptr<imagem> m_imagem)                           : area_de_toque(ptr) {
                m_filhos.push_back(std::move(m_imagem));
                old_fundo_alpha = m_estilo.m_cor_fundo.a;
            }
            botao(bool* ptr, std::unique_ptr<texto> m_texto)                           : area_de_toque(ptr) {
                m_filhos.push_back(std::move(m_texto));
                old_fundo_alpha = m_estilo.m_cor_fundo.a;
            }
            /// @}
            /// @name Construtores de texto simplificado
            /// @{
            botao(const std::function<void()> &fctn, const std::string& txt) : area_de_toque(fctn) {
                auto m_texto = std::make_unique<texto>(txt);
                m_filhos.push_back(std::move(m_texto));
                old_fundo_alpha = m_estilo.m_cor_fundo.a;
            }
            botao(bool* ptr, const std::string& txt) : area_de_toque(ptr) {
                auto m_texto = std::make_unique<texto>(txt);
                m_estilo.m_limites.z += m_texto->obterLargura(txt);
                m_filhos.push_back(std::move(m_texto));
                old_fundo_alpha = m_estilo.m_cor_fundo.a;
            }
            botao(bool* ptr, const std::string& txt, const std::string img = "", const unsigned int size = 20) : area_de_toque(ptr) {
                if(img.empty() == false) {
                auto m_imagem = std::make_unique<imagem>(img);
                m_imagem->m_estilo.m_altura = size;
                m_imagem->m_estilo.m_largura = size;
                m_filhos.push_back(std::move(m_imagem));
                }
                if(txt.empty()) return;
                auto m_texto = std::make_unique<texto>(txt, size);
                m_estilo.m_limites.z += m_texto->obterLargura(txt);
                m_filhos.push_back(std::move(m_texto));
            }
            botao(const std::function<void()> &fctn, const std::string& txt, const std::string img, const unsigned int size = 20) : area_de_toque(fctn) {
                auto m_imagem = std::make_unique<imagem>(img);
                m_imagem->m_estilo.m_altura = size;
                m_imagem->m_estilo.m_largura = size;

                m_estilo.m_limites.z = size;
                
                m_filhos.push_back(std::move(m_imagem));
                old_fundo_alpha = m_estilo.m_cor_fundo.a;

                if(txt.empty()) return;
                auto m_texto = std::make_unique<texto>(txt, size);
                m_estilo.m_limites.z += m_texto->obterLargura(txt);
                m_filhos.push_back(std::move(m_texto));
            }
            /// @}
            ~botao() {
            }
            void atualizar() override {
                m_estilo.m_cor_borda.a = mouseEmCima() ? old_fundo_alpha : old_fundo_alpha*0.1f;
                caixa::atualizar();
            }
            void configurar() override {
                m_estilo.m_flag_estilo |= tem(flag_estilo::largura_percentual) ? flag_estilo::modular : flag_estilo::largura_justa | flag_estilo::altura_justa | flag_estilo::modular;
                old_fundo_alpha = m_estilo.m_cor_fundo.a;
            }
        };  
    } // elementos
} // namespace becommons
