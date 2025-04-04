#include "componentes/transformacao.hpp"
#include "nucleo/fase.hpp"
#include "nucleo/projeto.hpp"
#include "os/janela.hpp"

void bubble::transformacao::apontarEntidade(const uint32_t& ent)
{
	if (!projeto_atual->obterFaseAtual()->obterRegistro()->tem<bubble::transformacao>(ent))return;
	alvo = &projeto_atual->obterFaseAtual()->obterRegistro()->obter<bubble::transformacao>(ent)->posicao;
}

void bubble::transformacao::apontarV3(const glm::vec3& pos)
{
    if(!alvo)
	*alvo = pos;
    else
        alvo = new glm::vec3(pos);
}
