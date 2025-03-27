#include "nucleo/sistema_de_interface.hpp"
#include "sistemas/bubble_gui.hpp"
#include "os/janela.hpp"
#include "depuracao/debug.hpp"

using namespace bubble;

sistema_interface s;

bubble_gui::bubble_gui()
{
    s.inicializar(nullptr);
    raiz = std::make_unique<caixa>();
    raiz->m_id = "raiz";
    caixas["raiz"] = raiz.get();
}

void bubble_gui::adicionarFlags(const std::string& id, flags f) {
    if (auto caixa = obterCaixa(id)) {
        caixa->m_flags |= f;
    }
}
void bubble_gui::atualizar()
{
    obterCaixa("raiz")->m_limites = {0.f, 0.f,
        static_cast<float>(instanciaJanela->tamanho.x),
        static_cast<float>(instanciaJanela->tamanho.y)};
    obterCaixa("raiz")->m_altura = static_cast<float>(instanciaJanela->tamanho.y);
    obterCaixa("raiz")->m_largura = static_cast<float>(instanciaJanela->tamanho.x);
    atualizarFilhos(obterCaixa("raiz"));
}

inline void desenhar_caixa(caixa* c)
{
    
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    if (c->m_imagem_fundo)
    {
        // Renderizar imagem
        c->m_imagem_fundo->padding = {static_cast<double>(c->m_limites.x), static_cast<double>(c->m_limites.y)};
        c->m_imagem_fundo->limite.x = c->m_limites.z;
        c->m_imagem_fundo->limite.y = c->m_limites.w;
        s.desenharImagem(*s.shader_imagem, *c->m_imagem_fundo);
    }

}

void bubble_gui::atualizarFilhos(caixa* it_caixa)
{
    if (!it_caixa) {
        return; // Evita acesso inválido caso a caixa não exista
    }

    // Se a caixa for modular, atualizar limites dos filhos
    if(it_caixa->tem_flag(flags::modular)){
        // passo 1: calcular métricas
        const bool is_horizontal = it_caixa->m_orientacao_modular == caixa::orientacao::horizontal;
        float espaco_disponivel = is_horizontal ? it_caixa->m_largura : it_caixa->m_altura;
        float crescimento_total = 0.0f;
        float espaco_fixo = 0.0f;
        
        // primeira passagem: calcular espaço fixo e crescimento total
        for(auto& filho : it_caixa->m_filhos)
        {
            if(is_horizontal) {
                if(filho->tem_flag(flags::largura_proporcional))
                {
                    // adicionar proporcionalmente
                    espaco_fixo += espaco_disponivel * filho->m_largura;
                } else {
                    // adiciona em pixels
                    espaco_fixo += filho->m_largura;
                }
            } else {
                if(filho->tem_flag(flags::altura_proporcional))
                {
                    espaco_fixo += espaco_disponivel * filho->m_altura;
                } else {
                    espaco_fixo += filho->m_altura;
                }
            }
            crescimento_total += filho->m_crescimento_modular;
        }
        // calcular espaço modular restante
        float espaco_restante = espaco_disponivel - espaco_fixo;
        espaco_restante = std::max(espaco_restante, 0.0f);
        float unidade_crescimento = crescimento_total > 0 ? espaco_restante / crescimento_total : 0;
        
        // segunda passagem: aplicar dimensões e proporções
        float cursor = 0.0f;
        for(auto& filho : it_caixa->m_filhos)
        {
            // calcular tamanho principal
            float tamanho_principal = 0.0f;
            if(is_horizontal) {
                if (filho->tem_flag(flags::largura_proporcional)) {
                    tamanho_principal =  espaco_disponivel * filho->m_largura;
                } else {
                    tamanho_principal = filho->m_largura;
                }
                tamanho_principal += filho->m_crescimento_modular * unidade_crescimento;

                // atualizar largura
                filho->m_limites.z = tamanho_principal;

                // posicionar horizontalmente
                filho->m_limites.x = cursor;
                cursor += tamanho_principal;

                // altura proporcional ao pai
                if(filho->tem_flag(flags::altura_proporcional)) {
                    filho->m_limites.w = it_caixa->m_altura * filho->m_altura;
                }
            } else {
                if (filho->tem_flag(flags::altura_proporcional)) {
                    tamanho_principal = espaco_disponivel * filho->m_altura;
                } else {
                    tamanho_principal = filho->m_altura;
                }
                tamanho_principal += filho->m_crescimento_modular * unidade_crescimento;
                
                // atualizar altura
                filho->m_limites.w = tamanho_principal;
                
                // posicionar verticalmente
                filho->m_limites.y = cursor;
                cursor += tamanho_principal;

                // largura proporcional ao pai
                if (filho->tem_flag(flags::largura_proporcional)) {
                    filho->m_limites.z = it_caixa->m_largura * filho->m_largura;
                }
            }
        }
    }
    // atualizar recursivamente e desenhar
    for (auto& filho : it_caixa->m_filhos) {
        atualizarFilhos(filho.get());
    }    
    desenhar_caixa(it_caixa);
}
