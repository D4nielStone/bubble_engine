/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
#include "util/caixa.hpp"
#include "arquivadores/fonte.hpp"
#include "arquivadores/shader.hpp"

namespace bubble{
    namespace elementos{
            enum class flags_texto : uint32_t
            {
                padrao              = 0,
                alinhamento_central = 1 << 0,
                alinhamento_fim     = 1 << 1
            };
        inline flags_texto operator|(flags_texto a, flags_texto b) {
            return static_cast<flags_texto>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
        }

        inline flags_texto operator&(flags_texto a, flags_texto b) {
            return static_cast<flags_texto>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
        }

        inline flags_texto& operator|=(flags_texto& a, flags_texto b) {
            a = a | b;
            return a;
        }
        class texto : public caixa
        {
        public:
            flags_texto m_texto_flags;
            shader* m_texto_shader;
            cor m_texto_cor{1.f, 1.f, 1.f, 1.f};
            float m_texto_escala;
            std::string m_texto_frase, m_texto_fonte;
            texto(const std::string frase,
                    const float escala = 1.f,
                    const flags_texto flags = flags_texto::padrao,
                    const std::string path_fonte = "consolas.ttf") : m_texto_frase(frase)
                , m_texto_fonte(path_fonte)
                , m_texto_escala(escala)
                , m_texto_shader(new shader("texto.vert", "texto.frag"))
                , m_texto_flags(flags)
            {
                m_largura = obterLargura(m_texto_frase);
            }
            float obterLargura(const std::string& frase) {
                auto& caracteres = bubble::gerenciadorFontes::obterInstancia().obter(m_texto_fonte);
                if (caracteres.empty()) {
                    return 0.0f;
                }

                float larguraTotal = 0.0f;
                for (char32_t c : frase) {
                    if(c == '\n') return larguraTotal;
                    auto it = caracteres.find(c);
                    if (it != caracteres.end()) {
                        const bubble::caractere& ch = it->second;
                        larguraTotal += (ch.avanco >> 6) * m_texto_escala;
                    }
                }
            return larguraTotal;
            }
        };
    } // elementos
} // bubble
