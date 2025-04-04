/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
#include "imagem.hpp"
#include "texto.hpp"
#include "os/janela.hpp"
#include "inputs/inputs.hpp"
#include <functional>

namespace bubble{
    namespace elementos{
        class botao : public caixa
        {
        private:
            bool gatilho = false;
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
                if(m_imagem)
                {
                    m_imagem->m_limites.z = m_largura;
                    m_imagem->m_limites.w = m_altura;
                    m_imagem->m_limites.x = m_limites.x;
                    m_imagem->m_limites.y = m_limites.y;
                }
                if(m_texto)
                {
                    m_altura = m_texto->m_texto_escala * 24 + m_padding_geral.y;
                    m_largura = m_texto->obterLargura(m_texto->m_texto_frase) + m_padding_geral.x;
                    
                    m_texto->m_limites.x = m_limites.x + m_padding_geral.x/2;
                    m_texto->m_limites.y = m_limites.y+ m_padding_geral.y/2;
                    m_texto->m_flags_caixa = m_flags_caixa;
                }
                if(!instanciaJanela->inputs.isKeyPressed("MouseE"))
                    gatilho = false;
                auto m = obterMouse();
                if(m.x > m_limites.x && m.x < m_limites.z + m_limites.x &&
                   m.y > m_limites.y && m.y < m_limites.w + m_limites.y)
                {
                    instanciaJanela->defCursor(janela::cursor::mao);
                    if(!gatilho && instanciaJanela->inputs.isKeyPressed("MouseE"))
                    {
                        if(m_interruptor) 
                        {
                            if(*m_interruptor) *m_interruptor = false;
                            else               *m_interruptor = true;
                        }else
                        m_botao_funcao();
                    }
                }
                if(!gatilho && instanciaJanela->inputs.isKeyPressed("MouseE"))
                    gatilho = true;
            }
        };
    } // elementos
} // bubble
