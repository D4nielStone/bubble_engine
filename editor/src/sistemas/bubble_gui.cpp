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
    ancoras[ancora]->ancora_a = id_a;
    ancoras[ancora]->ancora_b = id_b;
    ancoras[ancora]->ori_atual = ori;

    return std::pair(id_a, id_b);
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
    ancoras[ancora_principal]->limites = {0.f, 0.f, instanciaJanela->tamanho.x, instanciaJanela->tamanho.y};
    recursivo(ancora_principal);
}

void desenhar_caixa(caixa* c)
{
    if(c->imagem)
    {
        // renderizar Imagem
        c->imagem->padding = {static_cast<double>(c->limites.x), static_cast<double>(c->limites.y)};
        c->imagem->limite = {c->limites.z, c->limites.w};
        s.desenharImagem(*s.shader_imagem, *c->imagem);
    }
}

void bubble_gui::recursivo(const ID ancora)
{
    if (ancoras.find(ancora) == ancoras.end() || ancoras[ancora] == nullptr)
        return; // Evita acesso inválido caso a âncora não exista

    // Atualiza caixa
    auto c = &ancoras[ancora]->corpo;
    if(c->ajuste == caixa::cobrir)
    {
        c->limites = ancoras[ancora]->limites;
    }    

    desenhar_caixa(c);

    float proporcao = ancoras[ancora]->proporcao;

    if(ancoras[ancora]->ori_atual == ancora::horizontal){
    // Verifica e processa a âncora B
    if (ancoras[ancora]->ancora_b != 0 && ancoras.find(ancoras[ancora]->ancora_b) != ancoras.end())
    {
        auto& b = ancoras[ancora]->ancora_b;
        ancoras[b]->limites.x = ancoras[ancora]->limites.z * proporcao + ancoras[ancora]->limites.x;
        ancoras[b]->limites.y = ancoras[ancora]->limites.y;
        ancoras[b]->limites.z = ancoras[ancora]->limites.z * proporcao;
        ancoras[b]->limites.w = ancoras[ancora]->limites.w;
        recursivo(b);
    }

    // Verifica e processa a âncora A
    if (ancoras[ancora]->ancora_a != 0 && ancoras.find(ancoras[ancora]->ancora_a) != ancoras.end())
    {
        auto& a = ancoras[ancora]->ancora_a;
        ancoras[a]->limites.x = ancoras[ancora]->limites.x;
        ancoras[a]->limites.y = ancoras[ancora]->limites.y;
        ancoras[a]->limites.z = ancoras[ancora]->limites.z * proporcao;
        ancoras[a]->limites.w = ancoras[ancora]->limites.w;
        recursivo(a);
    }
    }
}

