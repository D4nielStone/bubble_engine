/** @copyright 
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
 * @file caixa.hpp
 */

#pragma once
#include <string>    
#include <memory>
#include <vector>
#include "vetor2.hpp"
#include "vetor3.hpp"
#include "cor.hpp"
#include "vetor4.hpp"
#include "becommons_namespace.hpp"

namespace BECOMMONS_NS {
    
    /// @enum flag_estilo
    /// Flags que definem o estilo da caixa. Também controlam o alinhamento e estilo dos filhos.
    enum class flag_estilo : uint8_t {
        nenhuma             = 0,
        largura_percentual  = 1 << 0,   // 1
        altura_percentual   = 1 << 1,   // 2
        modular             = 1 << 2,   // 4
        alinhamento_central = 1 << 3,
        alinhamento_fim     = 1 << 4,
        mesma_linha         = 1 << 5,
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

    struct estilo {
        /// @enum estilo::orientacao
        /// Horizontal ou vertical
        enum class orientacao : uint32_t {
            horizontal,
            vertical
        };
        flag_estilo         m_flag_estilo = flag_estilo::nenhuma;
        orientacao          m_orientacao_modular = orientacao::horizontal;  // como organiza seus filhos
        bool                m_ativo {true};
        bool                m_ligar_la {false};            // ligar altura-largura
        bool                m_ligar_al {false};            // ligar largura-altura
        float               m_largura = 20.f;
        float               m_altura = 20.f;
        float               m_crescimento_modular = 0.0f; // fator de crescimento em layouts modulares
        unsigned int        m_espessura_borda = 1;
        ivet2               m_padding {0, 0};
        ivet2               m_padding_geral {0, 0};
        fvet4               m_limites {0, 0, 20, 20};
        cor                 m_cor_borda {0.1f, 0.1f, 0.1f, 0.f};
        cor                 m_cor_fundo {0.5f, 0.5f, 0.5f, 0.f};
        
        // Operador de diferença (!=)
        bool operator!=(const estilo& other) const
        {
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
            if(other.m_crescimento_modular != m_crescimento_modular) {
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
     * @class caixa
     * @brief funciona como uma div em css
     */
    enum class tipo_caixa { base, texto, botao, imagem, caixa_de_texto };
    class caixa
    {
    public:
        /*  Dados  */
        caixa() = default;
        virtual ~caixa() {};
        virtual tipo_caixa tipo() const { return tipo_caixa::base; }

        // Verifica se uma flag está ativa
        inline bool tem_flag(flag_estilo flag) {
            return (static_cast<uint32_t>(m_estilo.m_flag_estilo) & static_cast<uint32_t>(flag)) != 0;
        }

        std::string m_id;        
        /// Estilo da caixa
        estilo m_estilo;
        /// Estilo antigo para comparação
        estilo m_estilo_antigo;

        /// Hierarquia
        std::vector<std::unique_ptr<caixa>> m_filhos;
        std::unordered_map<std::string, caixa*> m_mapa_filhos;
        
        template <typename T, typename ...Args>
        T* adicionarFilho(const std::string& id, Args&&... args) {
            auto nova_caixa = std::make_unique<T>(std::forward<Args>(args)...);
            nova_caixa->m_id = id;
            auto ptr = nova_caixa.get();
            m_filhos.push_back(std::move(nova_caixa));
            m_mapa_filhos[id] = ptr;
            return ptr;
        }
        
        caixa* obterFilho(const std::string& id) {
            auto it = m_mapa_filhos.find(id);
            if (it != m_mapa_filhos.end()) {
                return it->second;
            }
            return nullptr;
        }
        virtual void configurar() {};
    };
}
