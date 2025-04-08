/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file caixa.hpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#pragma once
#include "componentes/imagem.hpp"
#include "util/vetor4.hpp"
#include "util/vetor2.hpp"
#include <string>    

namespace bubble
{
        enum class flags_caixa : uint32_t {
            nenhuma             = 0,
            largura_percentual  = 1 << 0,   // 1
            altura_percentual   = 1 << 1,   // 2
            modular             = 1 << 2,   // 4
            alinhamento_central = 1 << 3,
            alinhamento_fim     = 1 << 4,
            largura_auto        = 1 << 5,
            altura_auto         = 1 << 6,
            mesma_linha         = 1 << 7,
            largura_justa       = 1 << 8,
            altura_justa        = 1 << 9
        };
        inline flags_caixa operator|(flags_caixa a, flags_caixa b) {
            return static_cast<flags_caixa>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
        }

        inline flags_caixa operator&(flags_caixa a, flags_caixa b) {
            return static_cast<flags_caixa>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
        }

        inline flags_caixa& operator|=(flags_caixa& a, flags_caixa b) {
            a = a | b;
            return a;
        }

    /**
     * @class caixa
     * @brief funciona como uma div em css
     *
     */
    class caixa
    {
        /*  Dados  */
        public:
        caixa() = default;
        virtual ~caixa() {};
        enum class orientacao {
            horizontal,
            vertical
        };
             
        // Verifica se uma flag está ativa
        inline bool tem_flag(flags_caixa flag) {
            return (static_cast<uint32_t>(m_flags_caixa) & static_cast<uint32_t>(flag)) != 0;
        }

        std::string m_id;
        flags_caixa m_flags_caixa = flags_caixa::nenhuma;
        caixa::orientacao m_orientacao_modular = caixa::orientacao::horizontal;
        
        // Estilo
        bool m_ativo {true};
        float m_largura = 20.f;
        float m_altura = 20.f;
        float m_crescimento_modular = 0.0f; // Fator de crescimento em layouts modulares
        unsigned int m_espessura_borda = 1;
        bubble::vetor2<int> m_padding = {0, 0};
        bubble::vetor2<int> m_padding_geral = {0, 0};
        vet4 m_limites  {0, 0, 20, 20};
        cor m_cor_borda {0.1f, 0.1f, 0.1f, 0.f};
        cor m_cor_fundo {0.5f, 0.5f, 0.5f, 0.f};
        
        // Hierarquia
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
    };
}
