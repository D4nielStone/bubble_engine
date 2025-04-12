/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file bubble_gui.cpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#include "namespace.hpp"
#include "sistemas/bubble_gui.hpp"
#include "os/janela.hpp"
#include "depuracao/debug.hpp"
#include <cmath>
#include <functional>

using namespace BECOMMONS_NS;

glm::mat4 proj(1.f);

inline void atualizarLJ(caixa* it_caixa)
{
    const bool is_horizontal = it_caixa->m_orientacao_modular == caixa::orientacao::horizontal;
    float mais_largo = 0.f; float mais_alto = 0.f;
    if(!it_caixa->m_ativo) return;
    for(auto& filho : it_caixa->m_filhos)
    {
        if(!filho->m_ativo) continue;
        if(!is_horizontal && it_caixa->tem_flag(flags_caixa::largura_justa))
        {
        // Calcular largura justa
        mais_largo = std::max(mais_largo, filho->m_largura + filho->m_padding.x*2 + it_caixa->m_padding_geral.x*2);
        // Definir dimensão da caixa pai
        it_caixa->m_largura = mais_largo;
        }
    }
}

// Atualizar Hierarquia De Traz pra Frente
inline void atualizarHDTF(caixa* it_caixa, std::function<void(caixa*)> func) {
    // Percorre os filhos de trás pra frente
    for (auto it = it_caixa->m_filhos.rbegin(); it != it_caixa->m_filhos.rend(); ++it) {
        // Chama recursivamente antes de atualizar a caixa atual
        atualizarHDTF(it->get(), func);
        
        func((*it).get());
    }
}

void bubble_gui::desenhar_caixa(caixa* c)
{
    if(!c->m_ativo) return;
    if(c->m_cor_fundo.a != 0)
    {
        renderizarFundo(c, quad_shader);
    }
    
    if(auto txt = dynamic_cast<elementos::texto*>(c)){
        // Renderizar texto 
        renderizarTexto(txt);
    }else
    if(auto btn = dynamic_cast<elementos::botao*>(c)){
        // Renderizar botao
        if(btn->pressionado())
            funcoes.push(btn->m_funcao);
        
        if(btn->m_imagem)
            renderizarImagem(btn->m_imagem.get());
        if(btn->m_texto)
            renderizarTexto(btn->m_texto.get());
    }else
    
    if(auto img = dynamic_cast<elementos::imagem*>(c)){
        // Renderizar imagem
        img->m_imagem_tamanho.x = img->m_limites.z; // o m_imagem_tamanho serve para de ponteiro para viewport da camera
        img->m_imagem_tamanho.y = img->m_limites.w;
        renderizarImagem(img);
    }

    for(auto& filho : c->m_filhos)
    {
        if(filho->m_ativo 
                && filho->m_limites.x < c->m_limites.x + c->m_limites.z
                && filho->m_limites.y < c->m_limites.y + c->m_limites.w)
        desenhar_caixa(filho.get());
    }
}

void BECOMMONS_NS::renderizarImagem(elementos::imagem* img)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, img->m_imagem_id);
    img->m_imagem_shader->use();
    img->m_imagem_shader->setVec2("quadrado.posicao", img->m_limites.x, img->m_limites.y);
    img->m_imagem_shader->setVec2("quadrado.tamanho", img->m_limites.z, img->m_limites.w);
    img->m_imagem_shader->setVec2("resolucao_textura", img->m_limites.z, img->m_limites.w);
    img->m_imagem_shader->setCor("cor_borda", img->m_cor_borda);
    img->m_imagem_shader->setInt("textura", 0);
    img->m_imagem_shader->setInt("tamanho_bordas", img->m_espessura_borda);
    img->m_imagem_shader->setBool("mostrar_bordas", img->m_cor_borda.a != 0);
    img->m_imagem_shader->setBool("flip", img->m_imagem_flip);
    img->m_imagem_shader->setMat4("projecao", glm::value_ptr(proj));
        /*  desenho  */
    glBindVertexArray(ret_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void BECOMMONS_NS::renderizarFundo(caixa* it_caixa, BECOMMONS_NS::shader* quad_shader)
{
    quad_shader->use();
    quad_shader->setVec2("quadrado.posicao", it_caixa->m_limites.x, it_caixa->m_limites.y);
    quad_shader->setVec2("quadrado.tamanho", it_caixa->m_limites.z, it_caixa->m_limites.w);
    quad_shader->setVec2("resolucao_textura", it_caixa->m_limites.z, it_caixa->m_limites.w);
    quad_shader->setCor("cor_borda", it_caixa->m_cor_borda);
    quad_shader->setCor("cor", it_caixa->m_cor_fundo);
    quad_shader->setInt("tamanho_bordas", it_caixa->m_espessura_borda);
    quad_shader->setBool("mostrar_bordas", it_caixa->m_cor_borda.a != 0);
    quad_shader->setMat4("projecao", glm::value_ptr(proj));
        /*  desenho  */
    glBindVertexArray(ret_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void BECOMMONS_NS::renderizarTexto(elementos::texto* tex)
{

        // activate corresponding render state	
        tex->m_texto_shader->use();
        tex->m_texto_shader->setCor("textColor", tex->m_texto_cor);
        tex->m_texto_shader->setMat4("projecao", glm::value_ptr(proj));

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(text_VAO);

        // iterate through all characters
        std::string::const_iterator c;
        auto& chs = gerenciadorFontes::obterInstancia().obter(tex->m_texto_fonte, tex->m_texto_escala);
        float y_linha = tex->m_texto_escala;
        float x_linha = tex->m_limites.x; 
        if(((uint32_t)tex->m_texto_flags & (uint32_t)elementos::flags_texto::alinhamento_central)!=0) 
        {
            x_linha += tex->m_limites.z / 2 - tex->obterLargura(tex->m_texto_frase)/2;
        }
        if(tex->m_texto_frase_ptr) tex->m_texto_frase = *tex->m_texto_frase_ptr;
        
        std::string texto_final = tex->m_texto_frase;

        for(char32_t ca : texto_final)
        {
            if(ca == '\n') {y_linha += tex->m_texto_escala; x_linha = tex->m_limites.x; continue;}
            
            if (chs.empty())
                return;
            caractere ch = chs.at(ca);
            
            float xpos = x_linha + ch.apoio.x;
            float ypos = tex->m_limites.y - ch.apoio.y + y_linha;

            if(y_linha > tex->m_limites.y + tex->m_limites.w) break;
            float w = ch.tamanho.x;
            float h = ch.tamanho.y;

            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 1.0f },
                { xpos,     ypos,       0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 0.0f },

                { xpos,     ypos + h,   0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 0.0f },
                { xpos + w, ypos + h,   1.0f, 1.0f }
            };
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.id);
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, text_VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x_linha += (ch.avanco >> 6);
        }
        glBindVertexArray(0);
    }
bubble_gui::bubble_gui()
{
    raiz = std::make_unique<caixa>();
    raiz->m_id = "raiz";
    caixas["raiz"] = raiz.get();
}

void bubble_gui::inicializar(fase* f)
{
    definirBuffers();
}

bubble_gui::~bubble_gui()
{
    if(quad_shader)
    delete quad_shader;
}

void bubble_gui::definirBuffers()
{
    quad_shader = new shader("imagem.vert", "quad.frag");

    /*--------------BUFFERS--------------*/
        /*---------------texto---------------*/
        glGenVertexArrays(1, &text_VAO);
        glGenBuffers(1, &text_VBO);
        glBindVertexArray(text_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, text_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        /*----------------quad---------------*/

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
    glCullFace(GL_FRONT);
    glDisable(GL_DEPTH_TEST);
    glViewport(0, 0,
instanciaJanela->tamanho.x,
instanciaJanela->tamanho.y
            );
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);

    instanciaJanela->defCursor(janela::cursor::seta);

    proj = glm::ortho(0.0, instanciaJanela->tamanho.x, instanciaJanela->tamanho.y, 0.0);
    
    raiz->m_limites = {0.f, 0.f,
        static_cast<float>(instanciaJanela->tamanho.x),
        static_cast<float>(instanciaJanela->tamanho.y)};


    while(!funcoes.empty())
    {
        auto f = funcoes.front();
        f();
        funcoes.pop();
    }

    // Ajusta largura de último-primeiro
    atualizarHDTF(raiz.get(), atualizarLJ);
    
    atualizarFilhos(raiz.get());
    desenhar_caixa(raiz.get());

    glCullFace(GL_BACK);
}

void bubble_gui::atualizarFilhos(caixa* it_caixa)
{
    if (!it_caixa || !it_caixa->m_ativo) {
        return; // Evita acesso inválido caso a caixa não exista
    }
    
    // Se a caixa for modular, atualizar limites dos filhos
    if(it_caixa->tem_flag(flags_caixa::modular)){
        // Define qual é a dimensão principal e a secundária
        // Se horizontal: principal = largura (z), secundária = altura (w)
        // Se vertical: principal = altura (w), secundária = largura (z)
        // Para dimensões justas
        const bool is_horizontal = it_caixa->m_orientacao_modular == caixa::orientacao::horizontal;

        float espaco_disponivel_principal = is_horizontal ? it_caixa->m_limites.z : it_caixa->m_limites.w;
        float espaco_disponivel_secundario = is_horizontal ? it_caixa->m_limites.w : it_caixa->m_limites.z; 
        // Variáveis para a dimensão principal
        float crescimento_total_principal = 0.0f;
        float espaco_fixo_principal = 0.0f;
        // Para a secundária – neste exemplo não usamos crescimento, mas o espaço fixo é calculado
        float espaco_fixo_secundario = 0.0f;
        
        // Primeira passagem: calcular espaço fixo e crescimento para a dimensão principal;
        // e espaço fixo para a secundária, conforme as flags

        for(auto& filho : it_caixa->m_filhos)
        {
            if(!filho->m_ativo) continue;
                    
            if(is_horizontal) {
                // Dimensão principal (largura)
                if(filho->tem_flag(flags_caixa::largura_percentual))
                    espaco_fixo_principal += espaco_disponivel_principal * filho->m_largura;
                else
                    espaco_fixo_principal += filho->m_largura;
                crescimento_total_principal += filho->m_crescimento_modular;
                
                // Dimensão secundária (altura)
                if(filho->tem_flag(flags_caixa::altura_percentual))
                    espaco_fixo_secundario += espaco_disponivel_secundario * filho->m_altura;
                else
                    espaco_fixo_secundario += filho->m_altura;
            } else {
                // Dimensão principal (altura)
                if(filho->tem_flag(flags_caixa::altura_percentual))
                    espaco_fixo_principal += espaco_disponivel_principal * filho->m_altura;
                else
                    espaco_fixo_principal += filho->m_altura;
                crescimento_total_principal += filho->m_crescimento_modular;
                
                // Dimensão secundária (largura)
                if(filho->tem_flag(flags_caixa::largura_percentual))
                    espaco_fixo_secundario += espaco_disponivel_secundario * filho->m_largura;
                else
                    espaco_fixo_secundario += filho->m_largura;
            }
        }
        
        // Calcular espaço modular restante para a dimensão principal
        float espaco_restante_principal = espaco_disponivel_principal - espaco_fixo_principal;
        espaco_restante_principal = std::max(espaco_restante_principal, 0.0f);
        float unidade_crescimento = (crescimento_total_principal > 0) ? (espaco_restante_principal /  crescimento_total_principal) : 0;
        
        // Segunda passagem: aplicar dimensões e posicionamento para ambas as direções
        if(is_horizontal) {

            float cursor_principal = it_caixa->m_limites.x;
            float cursor_secundario = it_caixa->m_limites.y;
            
            // Se a flag de alinhamento central estiver ativa, recalcula o cursor principal
            if(it_caixa->tem_flag(flags_caixa::alinhamento_central))
            {
                float total_tamanho_principal = 0.0f;
                // Calcula o tamanho total que os filhos ocuparão na dimensão principal, incluindo paddings
                for(auto& filho : it_caixa->m_filhos)
                {
                    if(!filho->m_ativo) continue;
                    float tamanho_principal = (filho->tem_flag(flags_caixa::largura_percentual) ? 
                        espaco_disponivel_principal * filho->m_largura : filho->m_largura)
                        + filho->m_crescimento_modular * unidade_crescimento;
                    // Considera o padding horizontal antes e depois do filho
                    total_tamanho_principal += filho->m_padding.x + tamanho_principal + filho->m_padding.x + it_caixa->m_padding_geral.x;
                }
                float inicio = it_caixa->m_limites.x - it_caixa->m_padding_geral.x / 2;
                // Ajusta o cursor para centralizar os filhos no espaço disponível
                cursor_principal = inicio + (espaco_disponivel_principal - total_tamanho_principal) / 2;
            }
            
            for(auto& filho : it_caixa->m_filhos)
            {
                if(!filho->m_ativo) continue;
                float tamanho_principal = 0.0f;
                float tamanho_secundario = 0.0f;
                
                // Para a largura (dimensão principal)
                if (filho->tem_flag(flags_caixa::largura_percentual))
                    tamanho_principal = espaco_disponivel_principal * filho->m_largura;
                else
                    tamanho_principal = filho->m_largura;
                tamanho_principal += filho->m_crescimento_modular * unidade_crescimento;
                
                // Para a altura (dimensão secundária)
                if (filho->tem_flag(flags_caixa::altura_percentual))
                    tamanho_secundario = espaco_disponivel_secundario * filho->m_altura;
                else
                    tamanho_secundario = filho->m_altura;
                
                // Posicionamento horizontal (eixo principal)
                cursor_principal += filho->m_padding.x + it_caixa->m_padding_geral.x;
                filho->m_limites.x = cursor_principal;
                    filho->m_limites.z = tamanho_principal;
                cursor_principal += tamanho_principal + filho->m_padding.x;
                
                // Posicionamento vertical (eixo secundário)
                filho->m_limites.y = cursor_secundario + filho->m_padding.y + it_caixa->m_padding_geral.y;
                    filho->m_limites.w = tamanho_secundario;
            }
        } else { // Orientação vertical
            // Inicializa o cursor vertical com o valor padrão
            float cursor_vertical = it_caixa->m_limites.y;
            
            // Se a flag de alinhamento central estiver ativa, recalcula o cursor vertical
            if(it_caixa->tem_flag(flags_caixa::alinhamento_central))
            {
                float total_tamanho_vertical = 0.0f;
                size_t j = 0;
                while(j < it_caixa->m_filhos.size()){
                    auto& filho = it_caixa->m_filhos[j];
                    if(!filho->m_ativo) { j++; continue; }
                    // Se é mesma linha
                    if(filho->tem_flag(flags_caixa::mesma_linha)){
                        float altura_max_grupo = 0.0f;
                        while(j < it_caixa->m_filhos.size() && it_caixa->m_filhos[j]->m_ativo &&
                              it_caixa->m_filhos[j]->tem_flag(flags_caixa::mesma_linha))
                        {
                            auto& f = it_caixa->m_filhos[j];
                            float altura_fixa = (f->tem_flag(flags_caixa::altura_percentual)) ?
                                (it_caixa->m_limites.w * f->m_altura) : f->m_altura;
                            float altura_total = altura_fixa + f->m_crescimento_modular * unidade_crescimento;
                            // Considera o padding vertical antes e depois
                            altura_max_grupo = std::max(altura_max_grupo, altura_total + f->m_padding.y);
                            j++;
                        }
                        total_tamanho_vertical += altura_max_grupo + it_caixa->m_padding_geral.y;
                    } else {
                        float tamanho_principal = (filho->tem_flag(flags_caixa::altura_percentual)) ?
                            it_caixa->m_limites.w * filho->m_altura : filho->m_altura;
                        tamanho_principal += filho->m_crescimento_modular * unidade_crescimento;
                        total_tamanho_vertical += filho->m_padding.y + it_caixa->m_padding_geral.y + tamanho_principal + filho->m_padding.y;
                        j++;
                    }
                }
                float inicio_vertical = it_caixa->m_limites.y - it_caixa->m_padding_geral.y / 2;
                // Disponível na dimensão vertical é a altura (w)
                cursor_vertical = inicio_vertical + (it_caixa->m_limites.w - total_tamanho_vertical) / 2;
            }
            
            size_t i = 0;
            while(i < it_caixa->m_filhos.size()){
                auto& filho = it_caixa->m_filhos[i];
                if(!filho->m_ativo) { i++; continue; }
                
                // Se o filho possuir a flag "mesma_linha", processa grupo horizontal
                if(filho->tem_flag(flags_caixa::mesma_linha)){
                    // Cursor horizontal para o grupo: inicia no limite esquerdo do pai + padding geral
                    float cursor_horizontal = it_caixa->m_limites.x + it_caixa->m_padding_geral.x;
                    float altura_max_grupo = 0.0f;
                    // Processa todos os filhos consecutivos com a flag "mesma_linha"
                    while(i < it_caixa->m_filhos.size() && it_caixa->m_filhos[i]->m_ativo &&
                          it_caixa->m_filhos[i]->tem_flag(flags_caixa::mesma_linha))
                    {
                        auto& f = it_caixa->m_filhos[i];
                        
                        // Cálculo da largura (dimensão principal para o grupo horizontal)
                        float largura_fixa = (f->tem_flag(flags_caixa::largura_percentual)) ?
                            (it_caixa->m_limites.z * f->m_largura) : f->m_largura;
                        float largura_total = largura_fixa + f->m_crescimento_modular * unidade_crescimento;
                        
                        // Posicionamento horizontal do filho
                        f->m_limites.x = cursor_horizontal + f->m_padding.x;
                            f->m_limites.z = largura_total;
                        cursor_horizontal += f->m_limites.z + f->m_padding.x;
                        
                        // Posicionamento vertical do grupo (todos na mesma linha)
                        f->m_limites.y = cursor_vertical + f->m_padding.y + it_caixa->m_padding_geral.y;
                        float altura_fixa = (f->tem_flag(flags_caixa::altura_percentual)) ?
                            (it_caixa->m_limites.w * f->m_altura) : f->m_altura;
                        float altura_total = altura_fixa + f->m_crescimento_modular * unidade_crescimento;
                            f->m_limites.w = altura_total;
                        
                        // Atualiza a altura máxima do grupo (considerando padding)
                        altura_max_grupo = std::max(altura_max_grupo, f->m_limites.w + f->m_padding.y);
                        
                        i++;
                    }
                    // Após processar o grupo, atualiza o cursor vertical para o próximo grupo
                    cursor_vertical += altura_max_grupo + it_caixa->m_padding_geral.y;
                } else {
                    // Processa individualmente o filho sem flag "mesma_linha"
                    cursor_vertical += filho->m_padding.y + it_caixa->m_padding_geral.y;
                    float tamanho_principal = (filho->tem_flag(flags_caixa::altura_percentual)) ?
                        it_caixa->m_limites.w * filho->m_altura : filho->m_altura;
                    tamanho_principal += filho->m_crescimento_modular * unidade_crescimento;
                    filho->m_limites.y = cursor_vertical;
                        filho->m_limites.w = tamanho_principal;
                    cursor_vertical += tamanho_principal + filho->m_padding.y;
                    
                    // Posicionamento horizontal padrão para filhos isolados
                    filho->m_limites.x = it_caixa->m_limites.x + it_caixa->m_padding_geral.x + filho->m_padding.x;
                    float tamanho_secundario = (filho->tem_flag(flags_caixa::largura_percentual)) ?
                        it_caixa->m_limites.z * filho->m_largura - it_caixa->m_padding_geral.x * 2 : filho->m_largura;
                        filho->m_limites.z = tamanho_secundario;
                    i++;
                }
            }
        }
    }
    
    // Atualiza recursivamente os filhos
    for (auto& filho : it_caixa->m_filhos) {
        atualizarFilhos(filho.get());
    }
}
