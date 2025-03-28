/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
#include "imagem.hpp"
#include "os/janela.hpp"
#include "inputs/inputs.hpp"
#include <functional>

namespace bubble{
    namespace elementos{
        class botao : public imagem
        {
        private:
            bool gatilho = false;
            std::function<void()> m_botao_funcao;
        public:
            botao(const std::string path, std::function<void()> f) : imagem(path), m_botao_funcao(f)
            {
            }
            void atualizarFuncao()
            {
                if(!instanciaJanela->inputs.isKeyPressed("MouseE"))
                    gatilho = false;
                auto m = obterMouse();
                if(m.x > m_limites.x && m.x < m_limites.z + m_limites.x &&
                   m.y > m_limites.y && m.y < m_limites.w + m_limites.y)
                {
                    instanciaJanela->defCursor(janela::cursor::mao);
                    if(!gatilho && instanciaJanela->inputs.isKeyPressed("MouseE"))
                    {
                        m_botao_funcao();
                    }
                }
                else
                    instanciaJanela->defCursor(janela::cursor::seta);
                if(!gatilho && instanciaJanela->inputs.isKeyPressed("MouseE"))
                    gatilho = true;
            }
        };
    } // elementos
} // bubble
