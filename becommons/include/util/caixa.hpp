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
 */
/**
 * \file caixa.hpp
 */

#pragma once
#include <string>    
#include <memory>
#include <vector>
#include "material.hpp"
#include "vetor2.hpp"
#include "vetor3.hpp"
#include "cor.hpp"
#include "vetor4.hpp"
#include <glm/glm.hpp>
#include "arquivadores/shader.hpp"
#include "inputs/inputs.hpp"
#include "becommons_namespace.hpp"

namespace BECOMMONS_NS {
    
    // \enum flag_estilo
    // Flags que definem o estilo da caixa. Também controlam o alinhamento e estilo dos filhos.
    enum class flag_estilo : uint8_t {
        nenhuma             = 0,
        largura_percentual  = 1 << 0,   // 1
        altura_percentual   = 1 << 1,   // 2
        modular             = 1 << 2,   // 4
        alinhamento_central = 1 << 3,
        alinhamento_fim     = 1 << 4,
        quebrar_linha       = 1 << 5,
        largura_justa       = 1 << 6,
        altura_justa        = 1 << 7    // max = 8.
    };

    inline flag_estilo operator|(flag_estilo a, flag_estilo b) {
        return static_cast<flag_estilo>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    }

    inline flag_estilo operator&(flag_estilo a, flag_estilo b) {
        return static_cast<flag_estilo>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
    }

    inline flag_estilo& operator|=(flag_estilo& a, flag_estilo b) {
        a = a | b;
        return a;
    }
    // Flag para detecções de mouse
	static unsigned int s_contagem_areas;

    struct estilo {
        // \enum estilo::orientacao
        // Horizontal ou vertical
        enum class orientacao : bool {
            horizontal,
            vertical
        };
        flag_estilo         m_flag_estilo = flag_estilo::modular;
        orientacao          m_orientacao_modular = orientacao::horizontal;  // como organiza seus filhos
        bool                m_ativo {true};
        bool                m_ligar_la {false};            // ligar altura-largura
        bool                m_ligar_al {false};            // ligar largura-altura
        float               m_largura = 20.f;
        float               m_altura = 20.f;
        unsigned int        m_espessura_borda = 4;
        ivet2               m_padding {0, 0};
        ivet2               m_padding_geral {0, 0};
        fvet4               m_limites {0, 0, 20, 20};
        cor                 m_cor_borda {0.1f, 0.1f, 0.1f, 0.f};
        cor                 m_cor_fundo {1.f, 1.f, 1.f, 0.f};
        
        // Operador de diferença (!=)
        bool operator!=(const estilo& other) const {
            if(other.m_limites != m_limites) {
                return true;
            }
            if(other.m_ativo != m_ativo) {
                return true;
            }
            if(other.m_ligar_la != m_ligar_la) {
                return true;
            }
            if(other.m_ligar_al != m_ligar_al) {
                return true;
            }
            if(other.m_largura != m_largura) {
                return true;
            }
            if(other.m_altura != m_altura) {
                return true;
            }
            if(other.m_espessura_borda != m_espessura_borda) {
                return true;
            }
            if(other.m_padding != m_padding) {
                return true;
            }
            if(other.m_padding_geral != m_padding_geral) {
                return true;
            }
            if(other.m_cor_borda != m_cor_borda) {
                return true;
            }
            if(other.m_cor_fundo != m_cor_fundo) {
                return true;
            }
            if(other.m_orientacao_modular != m_orientacao_modular) {
                return true;
            }
            if((other.m_flag_estilo & m_flag_estilo) == flag_estilo::nenhuma) {
                return true;
            }
            return false;
        }
        // Operador de igualdade (==)
        bool operator==(const estilo& other) const
        {
            return !(*this != other);
        }
    };
    /**
     * \class caixa
     * \brief funciona como uma div em css
     */
    enum class tipo_caixa : uint8_t {
        base, texto, botao, imagem, popup, caixa_de_texto 
    };
    class caixa {
    public:
        caixa() : m_projecao(1.f) {
            m_material.definirUniforme("quadrado", &m_estilo.m_limites);
            m_material.definirUniforme("cor_borda", &m_estilo.m_cor_borda);
            m_material.definirUniforme("cor", &m_estilo.m_cor_fundo);
            m_material.definirUniforme("tamanho_bordas", &m_estilo.m_espessura_borda);
            m_material.definirUniforme("projecao", &m_projecao);
        };

        virtual ~caixa() { };
        virtual tipo_caixa tipo() const { return tipo_caixa::base; };

        bool tem(const flag_estilo& flag) const {
            return static_cast<uint16_t>(m_estilo.m_flag_estilo & flag) != 0;
        }

        bool m_novo_projecao, m_mouse_cima { false };
        glm::mat4 m_projecao;
        std::unique_ptr<shader> m_shader {nullptr};
        material m_material;
        caixa* m_pai{nullptr};
        estilo m_estilo, m_estilo_antigo;
        std::vector<std::unique_ptr<caixa>> m_filhos;
        
        template <typename T, typename ...Args>
        T* adicionar(Args&&... args) {
            auto nova_caixa = std::make_unique<T>(std::forward<Args>(args)...);
            nova_caixa->m_pai = this;
            nova_caixa->configurar();
            auto* ptr = nova_caixa.get();
            m_filhos.push_back(std::move(nova_caixa));
            return ptr;
        }

        template <typename T>
        T* adicionar(T* nova_caixa) {
            nova_caixa->m_pai = this;
            nova_caixa->configurar();
            m_filhos.push_back(nova_caixa);
            return nova_caixa;
        }
        
        template <typename T>
        T* adicionar(std::unique_ptr<T> nova_caixa) {
            nova_caixa->m_pai = this;
            nova_caixa->configurar();
            auto* ptr = nova_caixa.get();
            m_filhos.push_back(std::move(nova_caixa));
            return ptr;
        }
        bool remover(caixa* alvo) {
            auto it = std::remove_if(m_filhos.begin(), m_filhos.end(),
                [alvo](const std::unique_ptr<caixa>& ptr) {
                    return ptr.get() == alvo;
                });
                
            if (it != m_filhos.end()) {
                m_filhos.erase(it, m_filhos.end());
                return true;
            }
            return false;
        }
        virtual void configurar() {
        };
        virtual void atualizar() {
        };
        // @returns Se está presionado ou não
        bool mouseEmCima() {
            if(m_pai)
            if(!m_estilo.m_ativo || !m_pai->m_estilo.m_ativo) return false;
            // vetor2 do mouse
            auto m = inputs::obterMousePos();
            m_mouse_cima = (m.x > m_estilo.m_limites.x && m.x < m_estilo.m_limites.z + m_estilo.m_limites.x &&
               m.y > m_estilo.m_limites.y && m.y < m_estilo.m_limites.w + m_estilo.m_limites.y);
            return m_mouse_cima;
        }
        virtual void desenhar(unsigned int ret_VAO) {
            if(!m_shader)m_shader = std::make_unique<shader>("imagem.vert", "quad.frag");
            m_material.usar(*m_shader);
            glBindVertexArray(ret_VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        };
    };
}
