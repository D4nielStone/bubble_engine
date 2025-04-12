/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file botao.hpp
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#pragma once
#include "namespace.hpp"
#include "imagem.hpp"
#include "texto.hpp"
#include "area_de_toque.hpp"
#include <memory>

namespace BECOMMONS_NS{
    namespace elementos{
        struct botao : area_toque
        {
            /// Ponteiros para aparência
            std::unique_ptr<imagem> m_imagem    { nullptr };
            std::unique_ptr<texto>  m_texto     { nullptr };
            /// @name Construtores com ponteiro para aparência
            /// @{
            botao(const std::function<void()> &fctn, std::unique_ptr<texto> txt)    : area_toque(fctn), m_texto(std::move(txt)) {
            }
            botao(const std::function<void()> &fctn, std::unique_ptr<imagem> img)   : area_toque(fctn), m_imagem(std::move(img)) {
            }
            botao(bool* ptr, std::unique_ptr<imagem> img)                           : area_toque(ptr), m_imagem(std::move(img)) {
            }
            /// @}
            /// @name Construtores de texto simplificado
            /// @{
            botao(const std::function<void()> &fctn, const std::string& txt) : area_toque(fctn), m_texto(std::make_unique<texto>(txt)) {
            }
            botao(bool* ptr, const std::string& txt) : area_toque(ptr), m_texto(std::make_unique<texto>(txt)) {
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
                    m_texto->m_flags_caixa = m_flags_caixa;
                }
                if(m_pressionado) m_cor_borda.a = 1.f;
                return area_toque::pressionado();
            }
        };  
    } // elementos
} // bubble
