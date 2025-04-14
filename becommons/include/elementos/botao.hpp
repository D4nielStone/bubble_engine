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
            /// Ponteiros para aparência
            std::unique_ptr<imagem> m_imagem    { nullptr };
            std::unique_ptr<texto>  m_texto     { nullptr };
            /// @name Construtores com ponteiro para aparência
            /// @{
            botao(const std::function<void()> &fctn, std::unique_ptr<texto> txt)    : area_de_toque(fctn), m_texto(std::move(txt)) {
            }
            botao(const std::function<void()> &fctn, std::unique_ptr<imagem> img)   : area_de_toque(fctn), m_imagem(std::move(img)) {
            }
            botao(bool* ptr, std::unique_ptr<imagem> img)                           : area_de_toque(ptr), m_imagem(std::move(img)) {
            }
            /// @}
            /// @name Construtores de texto simplificado
            /// @{
            botao(const std::function<void()> &fctn, const std::string& txt) : area_de_toque(fctn), m_texto(std::make_unique<texto>(txt)) {
            }
            botao(bool* ptr, const std::string& txt) : area_de_toque(ptr), m_texto(std::make_unique<texto>(txt)) {
            }
            /// @}
            ~botao() {
            }
            bool pressionado() override
            {
                m_cor_borda.a = 0.f;
                if(m_imagem)
                {
                    m_imagem->m_limites.z = m_largura;
                    m_imagem->m_limites.w = m_altura;
                    m_imagem->m_limites.x = m_limites.x;
                    m_imagem->m_limites.y = m_limites.y;
                }
                if(m_texto)
                {
                    m_altura = m_texto->m_texto_escala + m_padding_geral.y;
                    m_largura = m_texto->obterLargura(m_texto->m_texto_frase) + m_padding_geral.x;
                    
                    m_texto->m_limites.x = m_limites.x + m_padding_geral.x/2;
                    m_texto->m_limites.y = m_limites.y+ m_padding_geral.y/2;
                    m_texto->m_flag_estilo = m_flag_estilo;
                }
                m_pressionado = area_de_toque::pressionado();
                if(m_mouse_cima) m_cor_borda.a = 1.f;
                return m_pressionado;
            }
        };  
    } // elementos
} // bubble
