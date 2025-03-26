#include "nucleo/sistema_de_interface.hpp"
#include "sistemas/bubble_gui.hpp"
#include "os/janela.hpp"

using namespace bubble;

sistema_interface s;
std::pair<ID, ID> bubble_gui::dividir(const ID ancora, const ancora::orientacao ori)
{
    // Cria ancora
    proximo_id++;
    ID id_b = proximo_id;
    ancoras[id_b] = std::make_shared<bubble::ancora>();
    ancoras[id_b]->id = proximo_id;

    proximo_id++;
    ID id_a = proximo_id;
    ancoras[id_a] = std::make_shared<bubble::ancora>();
    ancoras[id_a]->id = proximo_id;

    // Adiciona ancora ao pai
    auto& ancora_pai = ancoras[ancora];
    ancora_pai->ancora_a = id_a;
    ancora_pai->ancora_b = id_b;
    ancora_pai->ori_atual = ori;

    return std::make_pair(id_a, id_b);
}

bubble_gui::bubble_gui()
{
    s.inicializar(nullptr);
    proximo_id = 1;
    ancora_principal = proximo_id;
    ancoras[ancora_principal] = std::make_shared<bubble::ancora>();
}

void bubble_gui::atualizar()
{
    // Atualiza os limites da ancora principal
    ancoras[ancora_principal]->limites = {0.f, 0.f, static_cast<float>(instanciaJanela->tamanho.x), static_cast<float>(instanciaJanela->tamanho.y)};
    recursivo(ancora_principal);
}

void desenhar_caixa(caixa* c)
{

        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
    if (c->imagem)
    {
        // Renderizar imagem
        c->imagem->padding = {static_cast<double>(c->limites.x), static_cast<double>(c->limites.y)};
        c->imagem->limite = {c->limites.z, c->limites.w};
        s.desenharImagem(*s.shader_imagem, *c->imagem);
    }
}

void bubble_gui::recursivo(const ID ancora)
{
    if (ancoras.find(ancora) == ancoras.end() || !ancoras[ancora]) {
        return; // Evita acesso inválido caso a âncora não exista
    }

    // Atualiza caixa
    auto& ancora_atual = ancoras[ancora];
    auto c = &ancora_atual->corpo;
    if (c->ajuste == caixa::cobrir)
    {
        c->limites = ancora_atual->limites;
    }

    desenhar_caixa(c);

    float proporcao = ancora_atual->proporcao;

    // Processa âncoras dependendo da orientação
    if (ancora_atual->ori_atual == ancora::horizontal)
    {
        // Verifica e processa a âncora B
        if (ancora_atual->ancora_b != 0 && ancoras.find(ancora_atual->ancora_b) != ancoras.end())
        {
            auto& b = ancoras[ancora_atual->ancora_b];
            b->limites.x = ancora_atual->limites.z * (1 - proporcao) + ancora_atual->limites.x;
            b->limites.y = ancora_atual->limites.y;
            b->limites.z = ancora_atual->limites.z * proporcao;
            b->limites.w = ancora_atual->limites.w;
            recursivo(ancora_atual->ancora_b);
        }

        // Verifica e processa a âncora A
        if (ancora_atual->ancora_a != 0 && ancoras.find(ancora_atual->ancora_a) != ancoras.end())
        {
            auto& a = ancoras[ancora_atual->ancora_a];
            a->limites.x = ancora_atual->limites.x;
            a->limites.y = ancora_atual->limites.y;
            a->limites.z = ancora_atual->limites.z * (1 - proporcao);
            a->limites.w = ancora_atual->limites.w;
            recursivo(ancora_atual->ancora_a);
        }
    }
    else if (ancora_atual->ori_atual == ancora::vertical)
    {
            auto& b = ancoras[ancora_atual->ancora_b];
            auto& a = ancoras[ancora_atual->ancora_a];
        if(ancora_atual->ancora_a != 0 && a->tamanho == ancora::fixo)
            proporcao = (ancora_atual->limites.w - a->limites.w) / ancora_atual->limites.w;
        if(ancora_atual->ancora_b != 0 && b->tamanho == ancora::fixo)
            proporcao = (ancora_atual->limites.w - b->limites.w) / ancora_atual->limites.w;

        // Verifica e processa a âncora B
        if (ancora_atual->ancora_b != 0 && ancoras.find(ancora_atual->ancora_b) != ancoras.end())
        {
            b->limites.x = ancora_atual->limites.x;
            b->limites.y = ancora_atual->limites.y + ancora_atual->limites.w * (1 - proporcao);
            b->limites.z = ancora_atual->limites.z;
            b->limites.w = ancora_atual->limites.w * proporcao;
            recursivo(ancora_atual->ancora_b);
        }

        // Verifica e processa a âncora A
        if (ancora_atual->ancora_a != 0 && ancoras.find(ancora_atual->ancora_a) != ancoras.end())
        {
            a->limites.x = ancora_atual->limites.x;
            a->limites.y = ancora_atual->limites.y;
            a->limites.z = ancora_atual->limites.z;
            a->limites.w = ancora_atual->limites.w * (1 - proporcao);
            recursivo(ancora_atual->ancora_a);
        }
    }
}
