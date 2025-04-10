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
#include <functional>
#include "namespace.hpp"
#include "imagem.hpp"
#include "texto.hpp"
#include "util/caixa.hpp"
#include "inputs/inputs.hpp"
#include "os/janela.hpp"

namespace BECOMMONS_NS{
    namespace elementos{
        class botao : public caixa
        {
        private:
            bool m_gatilho = false;
            bool* m_interruptor{nullptr};
            std::function<void()> m_botao_funcao;
        public:
            imagem* m_imagem{ nullptr };
            texto* m_texto{ nullptr };
            botao(std::function<void()> f, texto* txt) : m_botao_funcao(f), m_texto(txt)
            {
            }
            botao(std::function<void()> f, const std::string& txt) : m_botao_funcao(f), m_texto(new texto(txt))
            {
            }
            botao(std::function<void()> f, imagem* img) : m_botao_funcao(f), m_imagem(img)
            {
            }
            botao(bool* ptr, imagem* img) : m_interruptor(ptr), m_imagem(img)
            {
            }
            botao(bool* ptr, const std::string& txt) : m_interruptor(ptr), m_texto(new texto(txt))
            {
            }
            ~botao(){}
            void atualizarFuncao()
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
                if(!instanciaJanela->m_inputs.isKeyPressed("MouseE"))
                    m_gatilho = false;
                auto m = obterMouse();
                if(m.x > m_limites.x && m.x < m_limites.z + m_limites.x &&
                   m.y > m_limites.y && m.y < m_limites.w + m_limites.y)
                {
                    m_cor_borda.a = 1.f;
                    instanciaJanela->defCursor(janela::cursor::mao);
                    if(!m_gatilho && instanciaJanela->m_inputs.isKeyPressed("MouseE"))
                    {
                        if(m_interruptor) 
                        {
                            if(*m_interruptor) *m_interruptor = false;
                            else               *m_interruptor = true;
                        }else
                        m_botao_funcao();
                    }
                }
                if(!m_gatilho && instanciaJanela->m_inputs.isKeyPressed("MouseE"))
                    m_gatilho = true;
            }
        };  
    } // elementos
} // bubble
