/** @copyright Copyright (c) 2025 Daniel Oliveira */
#include "componentes/imagem.hpp"
#include "util/vetor4.hpp"
#include <string>    

namespace bubble
{
    /**
     * @struct caixa
     * @brief funciona como uma div em css
     *
     */

        enum class flags : uint32_t {
            nenhuma           = 0,
            largura_proporcional = 1 << 0,   // 1
            altura_proporcional  = 1 << 1,   // 2
            crescimento_vertical = 1 << 2,   // 4
            crescimento_horizontal = 1 << 3, // 8
            fixo_na_tela       = 1 << 4,    // 16
            scroll_vertical    = 1 << 5,    // 32
            scroll_horizontal  = 1 << 6,     // 64
            borda_visivel      = 1 << 7,     // 128
            modular            = 1 << 8      // 256
        };
        inline flags operator|(flags a, flags b) {
            return static_cast<flags>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
        }

        inline flags operator&(flags a, flags b) {
            return static_cast<flags>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
        }

        inline flags& operator|=(flags& a, flags b) {
            a = a | b;
            return a;
        }

    struct caixa
    {

        enum class orientacao {
            horizontal,
            vertical
        };
             
        // Verifica se uma flag está ativa
        inline bool tem_flag(flags flag) {
            return (static_cast<uint32_t>(m_flags) & static_cast<uint32_t>(flag)) != 0;
        }

        /*  Dados  */


        std::string m_id;
        flags m_flags = flags::modular;
        caixa::orientacao m_orientacao_modular = caixa::orientacao::horizontal;
        
        // Dimensões
        float m_largura = 0.0f;
        float m_altura = 0.0f;
        float m_crescimento_modular = 0.0f; // Fator de crescimento em layouts modulares
        vet4 m_limites {0, 0, 20, 20};
        // Aparência
        std::unique_ptr<imagem> m_imagem_fundo{nullptr};
        
        // Hierarquia
        std::vector<std::unique_ptr<caixa>> m_filhos;
        std::unordered_map<std::string, caixa*> m_mapa_filhos;
        
        caixa* adicionarFilho(const std::string& id) {
            auto nova_caixa = std::make_unique<caixa>();
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
