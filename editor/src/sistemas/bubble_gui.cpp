#include "sistemas/bubble_gui.hpp"
#include "os/janela.hpp"
#include "depuracao/debug.hpp"

using namespace bubble;
void bubble::renderizarImagem(shader* s, const unsigned int id, const bool flip, const vet4& limites)
{
    glm::mat4 proj = glm::ortho(0.0, instanciaJanela->tamanho.x, instanciaJanela->tamanho.y, 0.0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);
    s->use();
    s->setVec2("quadrado.posicao", limites.x, limites.y);
    s->setVec2("quadrado.tamanho", limites.z, limites.w);
    s->setCor("difusa", { 1.f, 1.f, 1.f, 1.f });
    s->setInt("textura", 0);
    s->setInt("flip", flip);
    s->setMat4("projecao", glm::value_ptr(proj));
        /*  desenho  */
    glBindVertexArray(ret_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

bubble_gui::bubble_gui()
{
    raiz = std::make_unique<caixa>();
    raiz->m_id = "raiz";
    caixas["raiz"] = raiz.get();
    
    /*--------------BUFFERS--------------*/
        // definiçãoo de dados do quadrado (posição e UV)
        float vertices[] = {
            // posição (x, y)    // UV (u, v)
            0.0f, 0.0f,        0.0f, 0.0f, // Inferior esquerdo
             1.0f, 0.0f,        1.0f, 0.0f, // Inferior direito
             1.0f,  1.0f,        1.0f, 1.0f, // Superior direito
            0.0f,  1.0f,        0.0f, 1.0f  // Superior esquerdo
        };

        // indices para formar dois triângulos
        unsigned int indices[] = {
            0, 1, 2, // primeiro triângulo
            2, 3, 0  // segundo triângulo
        };

        // geração de VAO, VBO e EBO
        glGenVertexArrays(1, &ret_VAO);
        glGenBuffers(1, &ret_VBO);
        glGenBuffers(1, &ret_EBO);

        // configuração do VAO
        glBindVertexArray(ret_VAO);

        // configuração do VBO (dados do quadrado)
        glBindBuffer(GL_ARRAY_BUFFER, ret_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // configuração do EBO (indices)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // configuração do atributo de posição (layout location = 0)
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // configuração do atributo UV (layout location = 1)
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
}

void bubble_gui::adicionarFlags(const std::string& id, flags_caixa f) {
    if (auto caixa = obterElemento(id)) {
        caixa->m_flags_caixa |= f;
    }
}
void bubble_gui::atualizar()
{
    obterElemento("raiz")->m_limites = {0.f, 0.f,
        static_cast<float>(instanciaJanela->tamanho.x),
        static_cast<float>(instanciaJanela->tamanho.y)};
    obterElemento("raiz")->m_altura = static_cast<float>(instanciaJanela->tamanho.y);
    obterElemento("raiz")->m_largura = static_cast<float>(instanciaJanela->tamanho.x);
    atualizarFilhos(obterElemento("raiz"));
}

inline void desenhar_caixa(caixa* c)
{
    // TODO desenho do fundo
    
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    if(auto img = dynamic_cast<elementos::imagem*>(c)){
        // Renderizar imagem
        img->m_imagem_tamanho.x = c->m_limites.z; // o m_imagem_tamanho serve para de ponteiro para viewport da camera
        img->m_imagem_tamanho.y = c->m_limites.w;
        renderizarImagem(img->m_imagem_shader, img->m_imagem_id, img->m_imagem_flip, img->m_limites);
    }
}

void bubble_gui::atualizarFilhos(caixa* it_caixa)
{
    if (!it_caixa) {
        return; // Evita acesso inválido caso a caixa não exista
    }

    // Se a caixa for modular, atualizar limites dos filhos
    if(it_caixa->tem_flag(flags_caixa::modular)){
        // passo 1: calcular métricas
        const bool is_horizontal = it_caixa->m_orientacao_modular == caixa::orientacao::horizontal;
        float espaco_disponivel = is_horizontal ? it_caixa->m_largura : it_caixa->m_altura;
        float crescimento_total = 0.0f;
        float espaco_fixo = 0.0f;
        
        // primeira passagem: calcular espaço fixo e crescimento total
        for(auto& filho : it_caixa->m_filhos)
        {
            if(is_horizontal) {
                if(filho->tem_flag(flags_caixa::largura_proporcional))
                {
                    // adicionar proporcionalmente
                    espaco_fixo += espaco_disponivel * filho->m_largura;
                } else {
                    // adiciona em pixels
                    espaco_fixo += filho->m_largura;
                }
            } else {
                if(filho->tem_flag(flags_caixa::altura_proporcional))
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
                if (filho->tem_flag(flags_caixa::largura_proporcional)) {
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
                if(filho->tem_flag(flags_caixa::altura_proporcional)) {
                    filho->m_limites.w = it_caixa->m_altura * filho->m_altura;
                }
            } else {
                if (filho->tem_flag(flags_caixa::altura_proporcional)) {
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
                if (filho->tem_flag(flags_caixa::largura_proporcional)) {
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
