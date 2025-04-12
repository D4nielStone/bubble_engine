/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file botao.hpp
 * @author Daniel O. dos Santos
 *
 * @licence MIT License
 */

#pragma once
#include <functional>
#include "namespace.hpp"
#include "util/caixa.hpp"
#include "inputs/inputs.hpp"
#include "os/janela.hpp"

namespace BECOMMONS_NS {
    namespace elementos {
        struct area_toque : caixa
        {
            /// Flags de ativação
            bool m_gatilho = false, m_pressionado;
            std::function<void()> m_funcao;
            bool* m_interruptor{nullptr};
            /// Construtor padrão
            area_toque(const std::function<void()> &fctn) : m_funcao(fctn) {};
            area_toque(bool* ptr = nullptr) : m_interruptor(ptr) {};
            /// Atualiza função de ativação
            /// @returns Se está presionado ou não
            virtual bool pressionado() {
                /// Reset do flag m_pressionado
                m_pressionado = false;
                /// Se não está pressionado, reseta gatilho
                if(!instanciaJanela->m_inputs.isKeyPressed("MouseE"))
                    m_gatilho = false;
                /// vetor2 do mouse
                auto m = obterMouse();
                /// Caso dentro do campo
                if(m.x > m_limites.x && m.x < m_limites.z + m_limites.x &&
                   m.y > m_limites.y && m.y < m_limites.w + m_limites.y)
                {
                    /// Define cursor para mão
                    instanciaJanela->defCursor(janela::cursor::mao);
                    /// Caso o gatilho esteja desativado e o mouse esquerdo tocado
                    if(!m_gatilho && instanciaJanela->m_inputs.isKeyPressed("MouseE")) {
                        /// Se m_interruptor é diferente de nullptr
                        if(m_interruptor) {
                            /// Lógica de inversão
                            if(*m_interruptor) *m_interruptor = false;
                            else               *m_interruptor = true;
                        } else /// Se interruptor é nulo, define m_pressionado como true
                            m_pressionado = true;
                    }
                }
                if(!m_gatilho && instanciaJanela->m_inputs.isKeyPressed("MouseE"))
                    m_gatilho = true;
                return m_pressionado;
            }
        };
    }
}
